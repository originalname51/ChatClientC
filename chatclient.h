/*
 * chatclient.h
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
 *      This is the header file to chatclient.c.
 */

#ifndef CHATCLIENT_H_
#define CHATCLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

/*
 * Handle will prepend the hand of the user.
 * */
typedef struct handle {
	char * username;
 	int uNameLength;
}handle;

const int    NET_CHAR_RECIEVE_INT = 4;		//How many characters are being recieved (int in ASCII)
const int	   NET_CHAR_SEND_INT    = 5;	//How many characters are being sent. (int in ASCII)
const int    MESSAGE_LENGTH       = 500;	//Message Length. Can go up to 999 and function as normal.
const int	   CHAR_OFFSET			= 100;	//offset used to send same # of characters always.

int     serverSetUp(char * serverName, char * portno);
void    recieveMessage(char * message, int sockfd);
void    sendMessage(char * message, int sockfd, handle * uname);
int     checkQuit(char * message);
handle* getUserName();

#endif /* CHATCLIENT_H_ */
