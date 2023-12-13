#!/usr/bin/python3
from random import randint
from init import netcat, server, send_command, receive_response, _login, wait_user

HOST = 'localhost'
# PORT = randint(6000, 7000) # always keeping the same port can cause bind() to fail. Why?
PORT = 1234
PASS ="jambon"
NICK ="a_nick"
USER ="a_username"
LOGIN = "a_login"
REAL_NAME = "Real Name"
CHANNEL = "#test"


# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=1)
def login(ncs):
	_login(ncs[0], PASS, NICK, LOGIN, REAL_NAME)

@netcat(HOST, PORT, num_connections=2000)
def chat_reception(ncs):
	wait_user(f"connect to {PORT} {PASS} {CHANNEL}")
	for i, nc in enumerate(ncs):
		_login(nc, PASS, f"{NICK}_{i}", LOGIN, REAL_NAME)
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

@netcat(HOST, PORT, num_connections=100)
def noisy_chat(ncs):
	for i, nc in enumerate(ncs):
		_login(nc, PASS, f"{NICK}_{i}", LOGIN, REAL_NAME)
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	wait_user(f"connect to {PORT} {PASS} {CHANNEL}")
	for i, nc in enumerate(ncs):
		msg = f"from client_{i}"
		send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")

# -- main ---------------------------------------------------------
@server(PORT, PASS)
def main():
	# login()
	chat_reception()
	# noisy_chat()

if __name__ == "__main__":
	main()
