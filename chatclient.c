/*
 * chatclient.c
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
 */
#include "chatclient.h"


int main(int argc, char *argv[])
{
	int sockfd;
	char rMessage[MESSAGE_LENGTH];
	char sMessage[MESSAGE_LENGTH];
	sockfd = serverSetUp(argv[1],atoi(argv[2]));

	while(1)
	{
	recieveMessage(rMessage, sockfd);
	sendMessage(sMessage, sockfd);
	recieveMessage(rMessage,sockfd);
	}
	return 0;
}

/*
 * Send one value to be parsed as an integer. This is string size. Next value is how many characters are being sent.
 * */
void sendMessage(char * message, int sockfd)
{
	int sendCount;
	char iterateChar = 'a';
	char number [6];

	memset(message, '\0', MESSAGE_LENGTH);

	sendCount = 0;
	message = "Hi to the Java Server from the C Client";
	while(iterateChar != '\0')
	{
		iterateChar = message[sendCount];
		sendCount++;
	}
	sendCount++;
	sendCount += 100;
	sprintf(number,"%d",sendCount);
	number[NET_CHAR_SEND_INT-1] = '\n';
	write(sockfd, number, NET_CHAR_SEND_INT);
	write(sockfd, message, (sendCount-100));

}

/*
 * Protocol : receive two messages. First message is size of string about to be sent. It is a value between 0-500 and
 * is determined by stringsize + 100 (to get a constant byte size of 4) Second message is string of size.
 * */
void recieveMessage(char * message, int sockfd)
{
	int index, n, filesize;
	memset(message, '\0', MESSAGE_LENGTH);

	/*
	 * This will read in 3 characters, between 101 and 601. This is because each message is 500 characters.
	 * */
    index = 0;
	while(index < NET_CHAR_RECIEVE_INT)
	 {
	     n = read(sockfd, &message[index], NET_CHAR_RECIEVE_INT - index);
        if (n < 0)
           perror("ERROR reading from socket");
     	 index+=n;
	 }
	message[NET_CHAR_RECIEVE_INT] = '\0';

	/*
	 * Manipulate the message to see value of string heading our way.
	 * */
	filesize  = atoi(message);
	filesize -= 100;


	/*
	 * Set size of index to 0, reset message, and receive the message from server.
	 * */
	index = 0;
	memset(message, '\0', MESSAGE_LENGTH);
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
