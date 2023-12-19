#!/usr/bin/python3
from define import *
from init import netcat, send_command, receive_response, wait_user, server, receive_all_responses, measure_time, insert_eof_randomly
import time
import modes

# -- tests --------------------------------------------------------
@netcat(HOST, PORT, num_connections=10)
def chat_reception(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	time.sleep(DELAY)
	msg = "Salut toé😍"
	send_command(ncs[0], f"PRIVMSG {CHANNEL} :{msg}")
	time.sleep(DELAY)
	for i, nc in enumerate(ncs[1:]):
		answer = receive_response(nc, msg)
		success = msg in answer
		if not success:
			print(f"client {i} couldn't receive msg.")
		assert(success)
	time.sleep(1)

@netcat(HOST, PORT, num_connections=500)
def noisy_chat(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	for i, nc in enumerate(ncs):
		msg = f"from client_{i}"
		send_command(nc, f"PRIVMSG {CHANNEL} :{msg}")
	time.sleep(1)

@netcat(HOST, PORT, num_connections=20)
def nasty_chat(ncs):
	msg = "Salut tabarnak esti comment ça va?"*10
	expected_msg = "Salut t*b*rn*k *st* comment ça va?"*10
	time.sleep(1)
	for i, nc in enumerate(ncs):
		send_command(nc, f"PRIVMSG {NICK}_{(i-1)%20} :{msg}")
	time.sleep(DELAY)
	for i, nc in enumerate(ncs):
		assert(receive_response(nc, expected_msg))

# initial replies
		
# -- main ---------------------------------------------------------
# @server(PORT, PASS)
def main():
	chat_reception()
	for _ in range(3):
		noisy_chat()
	nasty_chat()
	modes.operator()
	modes.key()
	modes.limit()
	modes.invite()
	modes.no_ext_msg()
	modes.secret()
	modes.change_topic()

if __name__ == "__main__":
	main()
