#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

SERVER="localhost"
PORT="6669"
PASS="test"
NICK="Nick"
USER="User"
CHANNEL="#test"


function check_server {
    if ! pgrep ircserv > /dev/null;
    then
        echo -e "${RED}Server is not running${RESET}"
        exit 1
    fi
}

function send_command {
    echo -e "$1\r\n" | nc $SERVER $PORT 
}

# Compile
# make

# Start server
./ircserv $PORT $PASS &
sleep 1
check_server


# Test commands
echo -e "${GREEN}Testing commands${RESET}"
send_command "NAMES $CHANNEL"
# send_command "LIST"

# Is server running
check_server

# Stop server
echo -e "${GREEN}Stopping server${RESET}"
kill -9 $(pgrep ircserv)
sleep 1


# Exit
exit 0
