# ft_irc

This chat server follows the [IRC protocol](https://modern.ircdocs.horse/) and was implemented as part of *our studies at 42.<br>
It is compatible with most IRC clients.<br>

Supported [modes](https://modern.ircdocs.horse/#oper-user-mode) are o, k, l, i, n, s, t. <br>
Supported [commands](https://modern.ircdocs.horse/#command) are PASS, NICK, USER, PING, PONG, QUIT, JOIN, PART, TOPIC, NAMES, LIST, INVITE, KICK, MODE, PRIVMSG, HELP. <br>
The server can also censor inappropriate words and a bot will kick recidivists. <br>

The tester asserts that hundreds of simultaneous connections are sucsessful, all clients receives the expected messages and specific functionnalities work as intended. <br>
[![irc-chat.gif](https://i.postimg.cc/X7nCg8ht/irc-chat.gif)](https://postimg.cc/jCk2xzRh)
 
*[Emile Gervais](https://github.com/emgervais), [Émile Le Sage](https://github.com/ele-sage), [François Martineau](https://github.com/francoismartineau)
