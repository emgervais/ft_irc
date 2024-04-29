# ft_irc

This chat server follows the [IRC protocol](https://modern.ircdocs.horse/) and was implemented as part of our studies at 42.<br>
It is compatible with most IRC clients.<br>

## Supported [commands](https://modern.ircdocs.horse/#command)

```
COMMANDS :
- INVITE
- JOIN
- KICK
- LIST
- NAMES
- PART
- TOPIC
- MODE
- NICK
- QUIT
- USER
- PING
- PONG
- PRIVMSG
- HELP
```

## Supported [modes](https://modern.ircdocs.horse/#oper-user-mode)

```
CHANNEL MODES :

for users :
    - o : channel operator
for channels :
    - i : invite only
    - n : outside privmsg disabled
    - p : private
    - t : topic locked
    - k : key locked
    - l : user limit
```

The server can also censor inappropriate words and a bot will kick recidivists. <br>
If for some reason a client sends messages in parts, the server can reassemble them when the command delimiter is received. <br>

The tester asserts that:
- Hundreds of simultaneous connections are sucsessful regardless of timing.
- All clients receives the expected messages
- Specific functionnalities work as intended. <br>

![alt text](https://i.postimg.cc/FzCG4BkC/irc-chat2.gif)
![alt text](https://i.postimg.cc/X7nCg8ht/irc-chat.gif)


Authors: [Emile Gervais](https://github.com/emgervais), [Émile Le Sage](https://github.com/ele-sage), [François Martineau](https://github.com/francoismartineau)
