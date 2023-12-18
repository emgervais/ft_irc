#!/usr/bin/python3
from init import netcat, send_command, receive_response, wait_user
from define import *

# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=500)
def chat_reception(ncs):
	wait_user(f"connect to {PORT} {PASS} {CHANNEL}")
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	wait_user(f"connect to {PORT} {PASS} {CHANNEL}")
	msg = "Salut toé😍"
	send_command(ncs[0], f"PRIVMSG {CHANNEL} :{msg}")
	for i, nc in enumerate(ncs[1:]):
		answer = receive_response(nc, msg)
		success = msg in answer
		if success:
			print(f"client {i} successfully received msg.")
		else:
			print(f"client {i} couldn't receive msg. ______________________ERROR___")
		assert(success)

@netcat(HOST, PORT, num_connections=1000)
def noisy_chat(ncs):
	for i, nc in enumerate(ncs):
		send_command(nc, f"JOIN {CHANNEL}")
	for i, nc in enumerate(ncs):
		msg = f"from client_{i}"
		send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")
		print("after send_command")

# -- main ---------------------------------------------------------
# @server(PORT, PASS)
def main():
	for _ in range(1000):
		noisy_chat()

if __name__ == "__main__":
	main()
