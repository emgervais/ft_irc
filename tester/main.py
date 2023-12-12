#!/usr/bin/python3
from random import randint
from init import netcat, server, send_command, receive_response, _login
from time import sleep

HOST = 'localhost'
# PORT = randint(6000, 7000) # always keeping the same port can cause bind() to fail. Why?
PORT = 1243
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

@netcat(HOST, PORT, num_connections=3)
def clients_chat(ncs):
	for i, nc in enumerate(ncs):
		_login(nc, PASS, f"{NICK}_{i}", LOGIN, REAL_NAME)
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	msg = "Salut toé😍"
	send_command(ncs[0], f"PRIVMSG {CHANNEL} :{msg}")
	input("connect")
	for nc in ncs[1:]:
		answer = receive_response(nc, msg)
		print(f"answer: {answer}")
		assert(msg in answer)

@netcat(HOST, PORT, num_connections=300)
def clients_chat2(ncs):
	for i, nc in enumerate(ncs):
		_login(nc, PASS, f"{NICK}_{i}", LOGIN, REAL_NAME)
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	sleep(1)
	input("press enter to send messages")
	for i, nc in enumerate(ncs):
		msg = f"from client_{i}"
		send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")

# -- main ---------------------------------------------------------
@server(PORT, PASS)
def main():
	# login()
	# clients_chat()
	clients_chat2()

if __name__ == "__main__":
	main()
