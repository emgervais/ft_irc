import random
import string
import time
from init import netcat, send_command, receive_response, receive_all_responses
from define import *

# Channel modes only set by operator
@netcat(HOST, PORT, num_connections=1)
def get_new_channel_name(ncs):
	def extract_channel_names(txt):
		words = txt.split()
		hashtag_words = [word for word in words if word.startswith("#")]
		return hashtag_words
	def generate_unique_name(existing_names):
		while True:
			new_hashtag = '#' + ''.join(random.choices(string.ascii_lowercase, k=5))
			if new_hashtag not in existing_names:
				return new_hashtag	
	send_command(ncs[0], "LIST", DELAY)
	txt = '\n'.join(receive_all_responses(ncs[0], False))
	names = extract_channel_names(txt)
	new_name = generate_unique_name(names)
	return new_name



# ----------------------------------------------------
# o: make user operator
@netcat(HOST, PORT, num_connections=3)
def operator(ncs):
	channel = get_new_channel_name()
	for nc in ncs:
		send_command(nc, f"JOIN {channel}", DELAY)
	# ncs[0] is op
	# ncs[0] makes 1 op
	send_command(ncs[0], f"MODE {channel} +o {NICK}_1", DELAY) 
	# ncs[1] should succeed
	# ncs[2] should receive error 482
	send_command(ncs[1], f"MODE {channel} +s", DELAY) 
	send_command(ncs[2], f"MODE {channel} +s", DELAY) 
	assert(not receive_response(ncs[1], "482"))
	time.sleep(1)
	# assert(receive_response(ncs[2], "482"))
	receive_all_responses(ncs[2], True)
	time.sleep(1)

# k: -k or k newPassword
@netcat(HOST, PORT, num_connections=2)
def key(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}", DELAY) 	
	key = "jambon"
	# ncs[0] is op
	send_command(ncs[0], f"MODE {CHANNEL} +k {key}", DELAY)
	
	send_command(ncs[1], f"JOIN {CHANNEL}", DELAY)
	assert(receive_response(ncs[1], "475"))

	send_command(ncs[1], f"JOIN {CHANNEL} {key}", DELAY)
	assert(not receive_response(ncs[1], "475"))

# l: Limit user count on Channel
@netcat(HOST, PORT, num_connections=3)
def limit(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +l 2", DELAY)
	send_command(ncs[1], f"JOIN {CHANNEL}", DELAY) 
	send_command(ncs[2], f"JOIN {CHANNEL}", DELAY) 
	assert(not receive_response(ncs[1], "471"))
	assert(receive_response(ncs[2], "471"))
	
	send_command(ncs[0], f"MODE {CHANNEL} -l", DELAY)
	send_command(ncs[2], f"JOIN {CHANNEL}", DELAY) 
	assert(not receive_response(ncs[2], "471"))

# i: (-i) invite only
@netcat(HOST, PORT, num_connections=3)
def invite(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +i", DELAY)
	send_command(ncs[1], f"JOIN {CHANNEL}", DELAY) 
	assert(receive_response(ncs[1], "473"))
	send_command(ncs[0], f"INVITE {NICK}_2 {CHANNEL}", DELAY)
	assert(receive_response(ncs[2], f":{NICK}_0 INVITE {NICK}_2 {CHANNEL}"))
	send_command(ncs[2], f"JOIN {CHANNEL}", DELAY) 
	assert(not receive_response(ncs[2], "473"))
	send_command(ncs[0], f"MODE {CHANNEL} -i", DELAY)
	send_command(ncs[1], f"JOIN {CHANNEL}", DELAY) 
	assert(not receive_response(ncs[1], "473"))

# n: no external message
@netcat(HOST, PORT, num_connections=2)
def no_ext_msg(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +n", DELAY)
	msg = "jambon"
	send_command(ncs[1], f"PRIVMSG {CHANNEL} {msg}", DELAY)
	assert(receive_response(ncs[1], "404"))
	send_command(ncs[0], f"MODE {CHANNEL} -n", DELAY)
	send_command(ncs[1], f"PRIVMSG {CHANNEL} {msg}", DELAY)
	assert(not receive_response(ncs[1], "404"))
	
# s: secret (LIST doesn't show the channel)
@netcat(HOST, PORT, num_connections=2)
def secret(ncs):
	send_command(ncs[0], f"JOIN {CHANNEL}") 
	send_command(ncs[0], f"MODE {CHANNEL} +s", DELAY)
	send_command(ncs[1], f"LIST", DELAY)
	assert(not receive_response(ncs[1], CHANNEL))
	send_command(ncs[0], f"MODE {CHANNEL} -s", DELAY)
	send_command(ncs[1], f"LIST", DELAY)
	assert(receive_response(ncs[1], CHANNEL))

# t: only operator can change the topic
@netcat(HOST, PORT, num_connections=2)
def change_topic(ncs):
	for nc in ncs:
		send_command(nc, f"JOIN {CHANNEL}")
	send_command(ncs[0], f"MODE {CHANNEL} +t", DELAY)
	topic = "new topic"
	send_command(ncs[1], f"TOPIC {CHANNEL} :{topic}", DELAY)
	assert(receive_response(ncs[1], "482"))
	send_command(ncs[0], f"MODE {CHANNEL} -t", DELAY)
	send_command(ncs[1], f"TOPIC {CHANNEL} :{topic}", DELAY)
	assert(not receive_response(ncs[1], "482"))
	# receive_all_responses(ncs[1], True)
# ----------------------------------------------------
