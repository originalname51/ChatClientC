/*
 * chatclient.c
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
 */
#include "chatclient.h"


int main(int argc, char *argv[])
{
	int sockfd; //, n, filesize, index;
	char newmessage[500];
	sockfd = serverSetUp(argv[1],atoi(argv[2]));
	recieveMessage(newmessage, sockfd);
	return 0;
}

/*
 * Protocol : receive two messages. First message is size of string about to be sent. It is a value between 0-500 and
 * is determined by stringsize + 100 (to get a constant byte size of 4) Second message is string of size.
 * */
void recieveMessage(char * message, int sockfd)
{
	int index, n, filesize;
	memset(message, '\0', 500);

	/*
	 * This will read in 3 characters, between 101 and 601. This is because each message is 500 characters.
	 * */
    index = 0;
	while(index < NET_CHAR_NUMBER)
	 {
	     n = read(sockfd, &message[index], NET_CHAR_NUMBER - index);
        if (n < 0)
           perror("ERROR reading from socket");
     	 index+=n;
	 }
	message[NET_CHAR_NUMBER] = '\0';

	/*
	 * Manipulate the message to see value of string heading our way.
	 * */
	filesize  = atoi(message);
	filesize -= 100;


	/*
	 * Set size of index to 0, reset message, and receive the message from server.
	 * */
	index = 0;
	memset(message, '\0', 500);
	while(index < filesize)
	 {
		n = read(sockfd, &message[index], (filesize+1)-index);
		if (n < 0)
			perror("ERROR reading from socket");
			index+=n;
	 }

   //print message from server. Add newline.
   printf("%s\n",message);

}

int serverSetUp(char * serverName, int portno)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		perror("ERROR opening socket");
	server = gethostbyname(serverName);

	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		perror("ERROR connecting");

	return sockfd;
}
