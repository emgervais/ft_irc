import subprocess
import os
import signal
from time import sleep

HOST = 'localhost'
PORT = 1233
PASS ="test"
NICK ="Nick"
USER ="User"
LOGIN = "Login"
REAL_NAME = "Real Name"
CHANNEL = "#test"

server_process = None
nc_process = None

# -- init ----
def start_server(port, password):
	global server_process
	try:
		server_process = subprocess.Popen(['./ircserv', str(port), password], preexec_fn=os.setsid)
		sleep(1)
		print("Server started.")
		return server_process
	except FileNotFoundError:
		print("Unable to start server. File not found.")
	except Exception as e:
		print(f"Error starting server: {e}")

def stop_server():
    global server_process
    if server_process:
        try:
            os.killpg(os.getpgid(server_process.pid), signal.SIGTERM)
            print("Server stopped.")
        except Exception as e:
            print(f"Error stopping server: {e}")
    else:
        print("Server was not started.")

def start_nc(host, port):
	global nc_process
	try:
		nc_process = subprocess.Popen(['nc', host, str(port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		sleep(1)
		return nc_process
	except FileNotFoundError:
		print("Netcat (nc) command not found. Please make sure it's installed and in your PATH.")
		return None

def send_command(text):
	global nc_process
	if nc_process:
		try:
			text += "\r\n"
			nc_process.stdin.write(text.encode())
			nc_process.stdin.flush()
			sleep(.2)
			return True
		except Exception as e:
			print(f"Error sending text: {e}")
	else:
		print("Netcat process not available.")
	return False

def netcat_connection(func):
	def wrapper():
		if not start_server(PORT, PASS):
			print("Can't start server.")
			exit(1)
		if not start_nc(HOST, PORT):
			print("Can't start nc.")
			stop_server()
			exit(1)
		func()
		input("Press enter to stop connection")
		nc_process.terminate()
		stop_server()
	return wrapper

# -- tests ----
@netcat_connection
def login():
	print("-- login ----")
	send_command(f"PASS {PASS}")
	send_command(f"NICK {NICK}")
	send_command(f"USER {LOGIN} 0 * {REAL_NAME}")
	send_command(f"PONG irc.{HOST}")

# -- main ----
if __name__ == "__main__":
	login()
