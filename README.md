# ft_irc

This chat server follows the [IRC protocol](https://modern.ircdocs.horse/) and was implemented as part of our studies at 42.<br>
It is compatible with most IRC clients.<br>

Supported [modes](https://modern.ircdocs.horse/#oper-user-mode) are o, k, l, i, n, s, t. <br>
Supported [commands](https://modern.ircdocs.horse/#command) are PASS, NICK, USER, PING, PONG, QUIT, JOIN, PART, TOPIC, NAMES, LIST, INVITE, KICK, MODE, PRIVMSG, HELP. <br>
The server can also censor inappropriate words and a bot will kick recidivists. <br>
If for some reason a client sends messages in parts, the server can reassemble them when the command delimiter is received. <br>

The tester asserts that:
- Hundreds of simultaneous connections are sucsessful regardless of timing.
- All clients receives the expected messages
- Specific functionnalities work as intended. <br>

![alt text](https://i.postimg.cc/FzCG4BkC/irc-chat2.gif)
![alt text](https://i.postimg.cc/X7nCg8ht/irc-chat.gif)


Authors: [Emile Gervais](https://github.com/emgervais), [Émile Le Sage](https://github.com/ele-sage), [François Martineau](https://github.com/francoismartineau)
