#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "ClassTCPclient.h"
#include "ClassGame.h"
using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

WSADATA	wsaData;
char	recvbuf[DEFAULT_BUFLEN];
int		iResult;
int		recvbuflen = DEFAULT_BUFLEN;

struct addrinfo *addressList = NULL;
struct addrinfo *ptr = NULL;
struct addrinfo hints;
int		iRcvdBytes;

const char   *serverName;
SOCKET	connectSocket = INVALID_SOCKET;
char	sendbuf[1000];      
ClassGame g;


ClassTCPclient::ClassTCPclient()
{
}


ClassTCPclient::~ClassTCPclient()
{
}

// Sends a message to the server
void ClassTCPclient::sendMessage(char* message)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	getaddrinfo(serverName, DEFAULT_PORT, &hints, &addressList);

	connectSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);

	//Active open
	connect(connectSocket, addressList->ai_addr, (int)addressList->ai_addrlen);

	iResult = send(connectSocket, message, (int)strlen(message), 0);

	freeaddrinfo(addressList);

	printf("Bytes sent: %ld \n", iResult);

	
}


char* ClassTCPclient::messageRecieved()
{
	//strcpy(recvbuf, "test");
	// Block waiting to receive response from server
	iResult = recv(connectSocket, recvbuf, recvbuflen, 0);

	// YOU must handle the streaming of the bytes being received!! e.g create a buffer to store the until you get enough to unpack and use

	if (iResult > 0)
	{
		// iResult is the number od bytes received. Assumed NUL char is included in recvbuf to terminate the string
		printf("Bytes received: %d msg: %s \n", iResult, recvbuf);
		iRcvdBytes = iResult;
	}
	else if (iResult == 0)
	{
		printf("Connection closed  by server");
	}
	else
	{
		printf("recv failed: %d \n", WSAGetLastError());
	}
	return recvbuf;

}



// Lobby
void ClassTCPclient::clientMain()
{
	char buff[256];
	char message[256];
	serverName = "127.0.0.1";
	
	//strcpy(message, "hello");
	//printf(">");
	//scanf("%s", &buff);//message to send and recieve
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	char rty[1000];
	sprintf(rty, "%s %i", "hello", clock());
	strcpy(message, " ");
	sendMessage(message);
	strcpy(recvbuf, messageRecieved());
	while (1)
	{

		// Type in number 
		printf(">");
		scanf("%s", &buff);//message to send and recieve
		printf("\n");
		sendMessage((char*)buff);

		//messageRecieved();

		strcpy(recvbuf, messageRecieved());
		printf("%s", recvbuf);
		
		// outpu sendbuf
		cin >> sendbuf;
		sendMessage(sendbuf);

		strcpy(buff, messageRecieved());
		
		printf("%s", buff);

		if (strcmp(sendbuf, "1") == 0)
		{
			// if 1 is selected then run game
			g.MultiPlayer();
		}
		else if (strcmp(sendbuf, "2") == 0)
		{
			// if 2 is selected then exit game
			exit(0);
		}
	}
	
	closesocket(connectSocket);


	printf("\r\nHit key to exit");
	while (!_kbhit());
	shutdown(connectSocket, SD_SEND);
	WSACleanup();
}


