from init import netcat_connection, server_management, send_command

HOST = 'localhost'
PORT = 1233
PASS ="test"
NICK ="Nick"
USER ="User"
LOGIN = "Login"
REAL_NAME = "Real Name"
CHANNEL = "#test"


# -- tests --------------------------------------------------------
@netcat_connection(HOST, PORT, num_connections=1)
def login(nc1):
	send_command(nc1, f"PASS {PASS}")
	send_command(nc1, f"NICK {NICK}")
	send_command(nc1, f"USER {LOGIN} 0 * {REAL_NAME}")
	send_command(nc1, f"PONG irc.{HOST}")


# -- main ---------------------------------------------------------
@server_management(PORT, PASS)
def main():
	login()

if __name__ == "__main__":
	main()
