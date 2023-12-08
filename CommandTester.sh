#!/usr/bin/python3

RED='\033[0;31m'
GREEN='\033[0;32m'
RESET_COLOR='\033[0m'

SERVER="localhost"
PORT="6669"
PASS="test"
NICK="Nick"
USER="User"
LOGIN="Login"
REAL_NAME="Real Name"
CHANNEL="#test"
CRLF=$'\r\n'

# -- server ----
function start_server {
    echo "-- start_server ----"
    ./ircserv $PORT $PASS &
    sleep 1
    check_server
}

function check_server {
    echo "-- check_server ----"
    if ! pgrep ircserv > /dev/null;
    then
        echo -e "${RED}Server is not running${RESET_COLOR}"
        exit 1
    fi
}

function stop_server {
    echo "-- stop_server ----"
    kill -9 $(pgrep ircserv)
    sleep 1
}
# --

# -- nc ----
function start_nc {
    exec 3<>/dev/tcp/$SERVER/$PORT
    nc -c <&3 >/dev/null 2>&1 &
    NC_PID=$!
    echo $NC_PID > nc_pid_file
    exec 3>&-  # Close file descriptor 3
}

function stop_nc {
    if [ -f nc_pid_file ]; then
        NC_PIDS=$(lsof -iTCP:$PORT -sTCP:LISTEN -Fn -c nc | awk '/^n/ {print substr($0, 2)}')
        if [ -n "$NC_PIDS" ]; then
            echo "$NC_PIDS" | while read -r NC_PID; do
                kill "$NC_PID"
                wait "$NC_PID" 2>/dev/null  # Wait for termination
            done
        else
            echo "No active process found for NC_PID: $NC_PIDS"
        fi
        rm nc_pid_file
    else
        echo "PID file (nc_pid_file) not found."
    fi
}






# --

# -- tests ----
function send_command {
    printf -- "sending: $CRLF$1$CRLF"
    printf -- "$1$CRLF" >&3
}

function login {
    echo "-- login ----"
    send_command "PASS $PASS"
    send_command "NICK $NICK"
    send_command "USER $LOGIN 0 * $REAL_NAME"
    send_command "PONG irc.$SERVER"
}

function misc {
    send_command "NAMES $CHANNEL"
    # send_command "LIST"
}

# -- main ----
start_server
start_nc
# sleep 1
# login
stop_nc
stop_server