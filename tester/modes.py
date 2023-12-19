from init import netcat, send_command, receive_response, receive_all_responses
from define import *
from time import sleep

# modes (only performed by operator)

# t: any user can change the topic

# o: make user operator
@netcat(HOST, PORT, num_connections=3)
def operator(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}", .5) 
	# ncs[0] is op
	# ncs[0] makes 1 op
	send_command(ncs[0], f"MODE {CHANNEL} +o {NICK}_1", .5) 
	# ncs[1] should succeed
	# ncs[2] should receive error 482
	send_command(ncs[1], f"MODE {CHANNEL} +s") 
	send_command(ncs[2], f"MODE {CHANNEL} +s", .5) 
	assert(not receive_response(ncs[1], "482"))
	assert(receive_response(ncs[2], "482"))

# k: -k or k newPassword
@netcat(HOST, PORT, num_connections=2)
def key(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}", .5) 	
	key = "jambon"
	# ncs[0] is op
	send_command(ncs[0], f"MODE {CHANNEL} +k {key}", .5)
	
	send_command(ncs[1], f"JOIN {CHANNEL}", .5)
	assert(receive_response(ncs[1], "475"))

	send_command(ncs[1], f"JOIN {CHANNEL} {key}", .5)
	assert(not receive_response(ncs[1], "475"))

# l: Limit user count on Channel
@netcat(HOST, PORT, num_connections=3)
def limit(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +l 2", .5)
	send_command(ncs[1], f"JOIN {CHANNEL}") 
	send_command(ncs[2], f"JOIN {CHANNEL}", .5) 
	assert(not receive_response(ncs[1], "471"))
	assert(receive_response(ncs[2], "471"))
	
	send_command(ncs[0], f"MODE {CHANNEL} -l", .5)
	send_command(ncs[2], f"JOIN {CHANNEL}", .5) 
	assert(not receive_response(ncs[2], "471"))

# i: (-i) invite only
@netcat(HOST, PORT, num_connections=3)
def invite(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +i", .5)
	send_command(ncs[1], f"JOIN {CHANNEL}", .5) 
	assert(receive_response(ncs[1], "473"))
	send_command(ncs[0], f"MODE {CHANNEL} -i", .5)
	send_command(ncs[1], f"JOIN {CHANNEL}", .5) 
	assert(not receive_response(ncs[1], "473"))

# n: no external message
@netcat(HOST, PORT, num_connections=2)
def no_ext_msg(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +n", .5)
	msg = "jambon"
	send_command(ncs[1], f"PRIVMSG {CHANNEL} {msg}", .5)
	assert(receive_response(ncs[1], "404"))
	send_command(ncs[0], f"MODE {CHANNEL} -n", .5)
	send_command(ncs[1], f"PRIVMSG {CHANNEL} {msg}", .5)
	assert(not receive_response(ncs[1], "404"))
	
# s: secret (LIST doesn't show the channel)
@netcat(HOST, PORT, num_connections=2)
def secret(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}", .5) 
	# send_command(ncs[0], f"MODE {CHANNEL} +s", .5)
	send_command(ncs[1], f"LIST", .5)
	receive_all_responses(ncs[1], True)
	pass
