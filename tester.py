import subprocess
from time import sleep

HOST = 'localhost'
PORT = 1233

# -- init ----
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
        if not start_nc(HOST, PORT):
            print("Can't start nc.")
            exit(1)
        func()
        input("Press enter to stop connection")
        nc_process.terminate()
    return wrapper

# -- tests ----
@netcat_connection
def login():
	print("-- login ----")
	send_command("PASS $PASS")
	send_command("NICK $NICK")
	send_command("USER $LOGIN 0 * $REAL_NAME")
	send_command("PONG irc.$SERVER")

# -- main ----
if __name__ == "__main__":
	login()
