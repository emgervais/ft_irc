#!/usr/bin/python3
from time import sleep
from init import netcat, server, send_command, login, receive_response, start_nc, wait_user
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

@netcat(HOST, PORT, num_connections=5)
def noisy_chat(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	# wait_user(f"connect to {PORT} {PASS} {CHANNEL}")
	# for i, nc in enumerate(ncs):
	# 	msg = f"from client_{i}"
	# 	send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")


# -- main ---------------------------------------------------------
# @server(PORT, PASS)
def main():
	for _ in range(10):
		noisy_chat()
		break

if __name__ == "__main__":
	main()
