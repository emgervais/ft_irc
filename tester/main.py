from random import randint
from init import netcat, server, send_command

HOST = 'localhost'
PORT = randint(6000, 7000)
PASS ="test"
NICK ="a_nick"
USER ="a_username"
LOGIN = "a_login"
REAL_NAME = "Real Name"
CHANNEL = "#test"


# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=1)
def login(nc1):
	send_command(nc1, f"PASS {PASS}")
	send_command(nc1, f"NICK {NICK}")
	send_command(nc1, f"USER {LOGIN} 0 * {REAL_NAME}")
	send_command(nc1, f"PONG irc.{HOST}")


# -- main ---------------------------------------------------------
@server(PORT, PASS)
def main():
	login()

if __name__ == "__main__":
	main()
