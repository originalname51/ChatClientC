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
//	sockfd = serverSetUp(argv[1],atoi(argv[2]));
	char * username;

	username = getUserName();

	printf("%s", username);
/*	while(1)
	{
		recieveMessage(rMessage, sockfd);
		if(checkQuit(rMessage) == 0){
			shutdown(sockfd,2);
			return 0;
		}
		sendMessage(sMessage, sockfd);
		if(checkQuit(sMessage) == 0){
			shutdown(sockfd,2);
			return 0;
		}
	}
	*/
	free(username);
	return 0;
}


char * getUserName()
{
char username[13];
char * returnName;
memset(username,'\0',10);
int  nameLength;
int  i;
printf("What is your username?\n");
fgets(username, 12, stdin);

for(i = 0; i < 11; i++ )
{
	if(username[i] == '\0')
	{
		break;
	}
	nameLength++;
}

returnName = malloc(sizeof(char) * nameLength+1);

for(i = 0; i < nameLength-1;i++)
{
	returnName[i] = username[i];
}

returnName[nameLength-1] = '>';
returnName[nameLength] = ' ';
returnName[nameLength+1] = '\0';
for(i = 0; i < nameLength;i++)
{
	printf("%d\n", returnName[i]);
}
return returnName;
}

int checkQuit(char * array)
{
	char checkArray[5] = "\\quit";
	char firstArray[5];
	int i;
	for(i = 0; i < 6; i++){
		firstArray[i] = array[i];
	}
	firstArray[5] = '\0';
	return (strcmp(firstArray,checkArray) == 0) ? 0:1;
}

/*
 * Send one value to be parsed as an integer.
 * This is string size.
 * Next value is how many characters are being sent.
 * */
void sendMessage(char * message, int sockfd)
{
	int sendCount;
	char iterateChar = 'a';
	char number [6];

	memset(message, '\0', MESSAGE_LENGTH);
	fgets(message, MESSAGE_LENGTH-1, stdin);

	sendCount = 0;
	while(iterateChar != '\0')
	{
		iterateChar = message[sendCount];
		sendCount++;
	}
	sendCount++;
	sendCount += CHAR_OFFSET;
	sprintf(number,"%d",sendCount);
	number[NET_CHAR_SEND_INT-1] = '\n';
	write(sockfd, number, NET_CHAR_SEND_INT);
	write(sockfd, message, (sendCount-CHAR_OFFSET));
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
	 * This will read in 4 characters(3 number characters + a newline), between 101 and 601. This is because each message is 500 characters.
	 * */
    index = 0;
	while(index < NET_CHAR_RECIEVE_INT)
	 {
	     n = read(sockfd, &message[index], NET_CHAR_RECIEVE_INT - index);
        if (n < 0)
           perror("ERROR reading from socket");
     	 index+=n;
	 }
	/*
	 * Manipulate the message to see value of string heading our way.
	 * */
	filesize  = atoi(message);
	filesize -= CHAR_OFFSET;

	/*
	 * Set size of index to 0, reset message, and receive the message from server.
	 * */
	index = 0;
	memset(message, '\0', MESSAGE_LENGTH);
	while(index < filesize)
	 {
		n = read(sockfd, &message[index], (filesize-index));
		if (n < 0)
			perror("ERROR reading from socket");
			index+=n;
	 }

   //print message from server.
   printf("%s",message);
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
