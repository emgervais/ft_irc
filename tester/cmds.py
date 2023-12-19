from init import netcat, send_command, receive_response, receive_all_responses
from define import *

@netcat(HOST, PORT, num_connections=1)
def names(ncs):
	pass
