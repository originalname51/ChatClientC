/*
 * chatclient.c
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
 *
 *      This program is a chat client made to connect to a Java server.
 */
#include "chatclient.h"


int main(int argc, char *argv[])
{
	int sockfd;
	char rMessage[MESSAGE_LENGTH];
	char sMessage[MESSAGE_LENGTH];
	sockfd = serverSetUp(argv[1],atoi(argv[2]));
	handle * username ;


	//Get the username and get in a loop.
	username = getUserName();
	printf("User Name Registered. Please send a message to the server when ready.\n");
	while(1)
	{
		//Send a message If it's sending \quit then shutdown the program.
		sendMessage(sMessage, sockfd, username);
		if(checkQuit(sMessage) == 0){
			shutdown(sockfd,2);
			free(username->username);
			free(username);
			return 0;
		}
		//Receive a message If it's receiving \quit then shutdown the program.
		recieveMessage(rMessage, sockfd);
		if(checkQuit(rMessage) == 0){
			shutdown(sockfd,2);
			free(username->username);
			free(username);
			return 0;
		}
	}
	//This should never return.
	return 0;
}

/*
 * This will prompt user for a username. It will then create a handle struct that
 * contains the length of the user name as well as the username itself.
 * */
handle * getUserName()
{
char username[13];
char * returnName;
memset(username,'\0',10);
int  nameLength;
int  i;
printf("What is your username?\n");
nameLength = 0;
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

//Set the returnName to the username.
for(i = 0; i < nameLength-1;i++)
{
	returnName[i] = username[i];
}
//Modify return name string to have handle.
returnName[nameLength-1] = '>';
returnName[nameLength] = ' ';
returnName[nameLength+1] = '\0';

//Make the userInfo struct.
handle *UInfo = malloc(sizeof(handle));
UInfo->uNameLength = nameLength;
UInfo->uNameLength++;
UInfo->username = returnName;
return UInfo;
}

//Check to see if quit was in last message.
int checkQuit(char * array)
{
	char checkArray[6] = "\\quit";
	char firstArray[6];
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
 * This code will prepend the user name and send a note to the server.
 * Message parameter will be changed.
 * */
void   sendMessage(char * message, int sockfd, handle * uname)
{
	int sendCount;
	char iterateChar = 'a';
	char number [6];
	char newChar[500]; //Create an array for the message that will be sent.

	memset(newChar, '\0', MESSAGE_LENGTH);	//set everything to end string.

	//prepend username
	int i = 0;
	for(i = 0; i < uname->uNameLength; i++)
	{
		newChar[i] = uname->username[i];
	}

	printf("%s",uname->username); //say username so client knows it can type.
	fgets(message, MESSAGE_LENGTH-uname->uNameLength, stdin); //get input.


	//This will count the number of characters that need to be send to the server.
	//It will also create a message (in newchar array) to be sent to the server.
	sendCount = 0;
	while(iterateChar != '\0')
	{
		newChar[uname->uNameLength+sendCount] = message[sendCount];
		iterateChar = message[sendCount];
		sendCount++;
	}
	sendCount+=uname->uNameLength;   //Add userName to the sendcount as it is characters.
	sendCount += CHAR_OFFSET;		//Add the offset to make sure it is a 3 digit character array.
	sprintf(number,"%d",sendCount);	//print the sendcount to a character array.
	number[NET_CHAR_SEND_INT-1] = '\n';

	//Send messages, first the integer amount (in characters), then send the message itself.
	write(sockfd, number, NET_CHAR_SEND_INT);
	write(sockfd, newChar, (sendCount-CHAR_OFFSET));
}

/*
 * Protocol : receive two messages. First message is size of string about to be sent. It is a value between 0-500 and
 * is determined by stringsize + 100 (to get a constant byte size of 4) Second message is string of size.
 *
 * This code will recieve a message from the server.
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
/*
 * Most of this code is derived from my operating system class Daemon project, following example code.
 * I followed this sockets tutorial as well.
 * http://www.cs.rpi.edu/~moorthy/Courses/os98/Pgms/socket.html
 *
 * This code connects to the server and will return the file descriptor.
 * */
int serverSetUp(char * serverName, int portno)
{
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//Make a socket. 0 makes it select TCP because it is a Sock Stream.
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		perror("ERROR opening socket");
		exit(2);
	}

	server = gethostbyname(serverName);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(2);
	}

	//Zero out the server address
	bzero((char *) &serv_addr, sizeof(serv_addr));

	//SetServer characteristics.
	serv_addr.sin_family = AF_INET; //always set for AF_INET.
	//Because this is a string it is required to be copied.
	bcopy((char *) server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);	//Set port number. Convert unsigned short integer to network byte order.

	//cast to sockaddr to avoid warning.
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		fprintf(stderr,"ERROR connecting. Is Server on?");
		exit(2);
	}
	return sockfd;
}
