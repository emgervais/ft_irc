import subprocess
import os
import signal
from time import sleep

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
	if not server_process:
		print("Server was not started.")
		return
	try:
		os.killpg(os.getpgid(server_process.pid), signal.SIGTERM)
		print("Server stopped.")
	except Exception as e:
		print(f"Error stopping server: {e}")

def start_nc(host, port):
	try:
		nc_process = subprocess.Popen(['nc', host, str(port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		sleep(1)
		return nc_process
	except FileNotFoundError:
		print("Netcat (nc) command not found. Please make sure it's installed and in your PATH.")
		return None

def send_command(nc_process, text):
	if not nc_process:
		print("Netcat process not available.")
		return False
	try:
		text += "\r\n"
		nc_process.stdin.write(text.encode())
		nc_process.stdin.flush()
		sleep(.2)
		return True
	except Exception as e:
		print(f"Error sending text: {e}")

def receive_response(nc_process, keyword=""):
	if not nc_process:
		print("Netcat process not available.")
		return None
	try:
		while True:
			output = nc_process.stdout.readline().decode().strip()
			if not keyword or (keyword and keyword.lower() in output.lower()):
				break
		return output
	except Exception as e:
		print(f"Error receiving response: {e}")
		return None

# -- decorators ----
def netcat(host, port, num_connections):
	def decorator(func):
		def wrapper():
			ncs = [start_nc(host, port) for _ in range(num_connections)]
			if all(ncs):
				func(*ncs)
				input("Press enter to stop connections")
				for nc in ncs:
					nc.terminate()
		return wrapper
	return decorator

def server(port, password):
    def decorator(func):
        def wrapper():
            if not start_server(port, password):
                print("Can't start server.")
                exit(1)
            func()
            stop_server()
        return wrapper
    return decorator
