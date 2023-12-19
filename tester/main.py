#!/usr/bin/python3
from define import *
from init import netcat, send_command, receive_response, wait_user, server
import modes

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
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	for i, nc in enumerate(ncs):
		msg = f"from client_{i}"
		send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")
		print("after send_command")

@netcat(HOST, PORT, num_connections=1000)
def nasty_chat(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	msg = "Salut tabarnak d'esti, comment ça va?"
	expected_msg = "Salut t*barnak d'*sti, comment ça va?"
	send_command(ncs[0], f"PRIVMSG {CHANNEL} :{msg}", DELAY)
	for i, nc in enumerate(ncs[1:]):
		success = False
		if receive_response(nc, expected_msg):
			print(f"client {i} successfully received msg.")
			success = True
		else:
			print(f"client {i} couldn't receive msg. ______________________ERROR___")
		assert(success)


# initial replies

# user: i (invisible) (pas de param)

# cmd names
		
# swearWords

# -- main ---------------------------------------------------------
# @server(PORT, PASS)
def main():
	# modes.get_new_channel_name()
	modes.operator()
	# modes.key()
	# modes.limit()
	# modes.invite()
	# modes.no_ext_msg()
	# modes.secret()
	# modes.change_topic()
	# for _ in range(1000):
		# noisy_chat()

if __name__ == "__main__":
	main()
