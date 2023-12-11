from random import randint
from init import netcat, server, send_command, receive_response
from time import sleep

HOST = 'localhost'
PORT = randint(6000, 7000)
PASS ="test"
NICK ="a_nick"
USER ="a_username"
LOGIN = "a_login"
REAL_NAME = "Real Name"
CHANNEL = "#test"

def _login(nc, passw, nick, login, real_name):
	send_command(nc, f"PASS {passw}")
	send_command(nc, f"NICK {nick}")
	send_command(nc, f"USER {login} 0 * :{real_name}")
	_pong(nc)
	
def _pong(nc):
	ping = receive_response(nc, "PING")
	send_command(nc, ping.replace("PING", "PONG"))

# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=1)
def login(nc1):
	_login(nc1, PASS, NICK, LOGIN, REAL_NAME)

@netcat(HOST, PORT, num_connections=2)
def connect_two_clients(nc1, nc2):
	_login(nc1, PASS, NICK, LOGIN, REAL_NAME)
	_login(nc2, PASS, NICK+"_2", LOGIN, REAL_NAME)
	send_command(nc1, f"JOIN {CHANNEL}")
	send_command(nc2, f"JOIN {CHANNEL}")
	
	msg = "Salut toé"
	send_command(nc2, f"PRIVMSG {CHANNEL} :{msg}")
	answer = receive_response(nc1, msg)
	print(f"msg: {msg}\nanswer: {answer}")


# -- main ---------------------------------------------------------
@server(PORT, PASS)
def main():
	# login()
	connect_two_clients()

if __name__ == "__main__":
	main()
