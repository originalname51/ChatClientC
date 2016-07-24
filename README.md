# ChatClientC
Chat client implemented in C.

This program will first establish a connection with a Server on at address arg[0] and port arg[1].

It will send the first message to the server.

Protocol is:

1)Send an array of characters to server with ASCII value of message(e.g. 124).

2)Send Message to Server.

To get the client running do the following.

1) Make sure the server is up and running.

2) Navigate into the C_Client folder.

3) Type "make"

4) Type "./chatclient [HOSTNAME] [PORTNUMBER]"

5) When you are finished conversing with the server type "\quit".
