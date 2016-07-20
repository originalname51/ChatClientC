/*
 * chatclient.c
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
 */
#include "chatclient.h"

int main(int argc, char *argv[])
{
	int sockfd, n, filesize, index;
	char newmessage[500];
	sockfd = serverSetUp(argv[1],atoi(argv[2]));
    n = write(sockfd, "abc", strlen("abc"));

    filesize=4;
    index = 0;
	while(index < filesize)
	 {
	     n = read(sockfd, &newmessage[index], filesize-index);
        if (n < 0)
           perror("ERROR reading from socket");
     	 index+=n;

     	 printf("%d", n);
	 }

	 printf("%s", newmessage);

	return 0;
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
