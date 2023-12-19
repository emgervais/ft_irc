import subprocess
import select
import os
import fcntl
import signal
import time
from define import *

# -- init ----
def start_server(port, password):
	kill_processes_by_name(NAME)
	global server_process
	try:
		server_process = subprocess.Popen([f'./{NAME}', str(port), password], preexec_fn=os.setsid)
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

def make_non_blocking(fd):
    flags = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, flags | os.O_NONBLOCK)

def start_nc(host, port):
	try:
		nc_process = subprocess.Popen(['nc', host, str(port)], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		make_non_blocking(nc_process.stdout.fileno())
		return nc_process
	except FileNotFoundError:
		print("Netcat (nc) command not found. Please make sure it's installed and in your PATH.")
		return None

def send_command(nc_process, text, sleep=0):
	if not nc_process:
		print("Netcat process not available.")
		return False
	try:
		text += "\r\n"
		nc_process.stdin.write(text.encode())
		nc_process.stdin.flush()
		if sleep:
			time.sleep(sleep)
		return True
	except Exception as e:
		print(f"Error sending text: {e}")

def receive_response(nc_process, keyword="", print_output=False):
	if not nc_process:
		print("Netcat process not available.")
		return None
	try:
		while True:
			output = nc_process.stdout.readline().decode().strip()
			if print_output:
				print("output:", output)
			if not output or not keyword \
			   or (keyword and keyword.lower() in output.lower()):
				break
		if keyword not in output:
			return ""
		return output
	except Exception as e:
		print(f"Error receiving response: {e}")
		return None

def receive_all_responses(nc_process, print_output):
	if not nc_process:
		print("Netcat process not available.")
		return None
	res = []
	try:
		while True:
			output = nc_process.stdout.readline().decode().strip()
			if print_output:
				print("output:", output)
			if not output:
				break
			res.append(output)
	except Exception as e:
		print(f"Error receiving response: {e}")
	return res

def kill_processes_by_name(name):
	try:
		pids = subprocess.check_output(["pgrep", name]).decode().split()
		for pid in pids:
			subprocess.run(["kill", pid])
		print(f"Killed processes with name '{name}'")
	except subprocess.CalledProcessError as e:
		no_process_found = (e.returncode == 1)
		if "pgrep: command not found" in e.output.decode():
			print("pgrep command not found. Please install pgrep.")
		elif no_process_found:
			pass
		else:
			print(f"Error: {e}")

# -- decorators ----
def netcat(host, port, num_connections):
	def decorator(func):
		def wrapper():
			ncs = [start_nc(host, port) for _ in range(num_connections)]
			for i, nc in enumerate(ncs):
				login(nc, PASS, f"{NICK}_{i}", LOGIN, REAL_NAME)
			if all(ncs):
				res = func(ncs)
				for nc in ncs:
					nc.terminate()
				return res
		return wrapper
	return decorator

def server(port, password):
	def decorator(func):
		def wrapper():
			if not start_server(port, password):
				print("Can't start server.")
				exit(1)
			func()
			time.sleep(1)
			stop_server()
		return wrapper
	return decorator

# -- sub functions ----
def login(nc, passw, nick, login, real_name):
	send_command(nc, f"PASS {passw}")
	send_command(nc, f"NICK {nick}")
	send_command(nc, f"USER {login} 0 * :{real_name}")
	_pong(nc)
	
def _pong(nc):
	ping = receive_response(nc, "PING")
	send_command(nc, ping.replace("PING", "PONG"))

# -- util ----
def wait_user(msg):
	time.sleep(1)
	input(msg)
	