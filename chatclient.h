/*
 * chatclient.h
 *
 *  Created on: Jul 19, 2016
 *      Author: rob
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
#include  <fcntl.h>

int    NET_CHAR_RECIEVE_INT = 4;
int	   NET_CHAR_SEND_INT    = 5;
int    MESSAGE_LENGTH       = 500;
int	   CHAR_OFFSET			= 100;

int    serverSetUp(char * serverName, int portno);
void   recieveMessage(char * message, int sockfd);
void   sendMessage(char * message, int sockfd);
int    checkQuit(char * message);
char * getUserName();

#endif /* CHATCLIENT_H_ */
