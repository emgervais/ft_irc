from random import randint
from init import netcat, server, send_command, receive_response, _login
from time import sleep

HOST = 'localhost'
PORT = randint(6000, 7000) # always keeping the same port can cause bind() to fail
# PORT = 6666
PASS ="test"
NICK ="a_nick"
USER ="a_username"
LOGIN = "a_login"
REAL_NAME = "Real Name"
CHANNEL = "#test"



# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=1)
def login(ncs):
	_login(ncs[0], PASS, NICK, LOGIN, REAL_NAME)

@netcat(HOST, PORT, num_connections=2)
def connect_two_clients(ncs):
	_login(ncs[0], PASS, NICK, LOGIN, REAL_NAME)
	_login(ncs[1], PASS, NICK+"_2", LOGIN, REAL_NAME)
	send_command(ncs[0], f"JOIN {CHANNEL}")
	send_command(ncs[1], f"JOIN {CHANNEL}")
	
	msg = "Salut toé"
	send_command(ncs[1], f"PRIVMSG {CHANNEL} :{msg}")
	answer = receive_response(ncs[0], msg)
	print(f"msg: {msg}\nanswer: {answer}")


# -- main ---------------------------------------------------------
@server(PORT, PASS)
def main():
	# login()
	connect_two_clients()

if __name__ == "__main__":
	main()
