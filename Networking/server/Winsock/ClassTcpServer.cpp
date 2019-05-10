#include "pch.h"
#include "ClassTCPserver.h"
#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>


using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

// Initialize winsock
WSADATA	wsaData;
char	recvbuf[DEFAULT_BUFLEN];
int		iResult;
int		recvbuflen = DEFAULT_BUFLEN; 
int		iRcvdBytes = 1000;

struct addrinfo *addressList = NULL;
struct addrinfo *ptr = NULL;
struct addrinfo hints;

// Create a socket
SOCKET ListenSocket = INVALID_SOCKET;
SOCKET ClientSocket = INVALID_SOCKET;

ClassTCPserver::ClassTCPserver()
{
}


ClassTCPserver::~ClassTCPserver()
{
}

// Sends message to the client 
void ClassTCPserver::sendMessage(char* message)
{
	iResult = send(ClientSocket, message, (int)strlen(message) + 1, 0);
	shutdown(ClientSocket, SD_SEND); // Forces message to send
}

// Message recieved from client
char* ClassTCPserver::messageRecieved()
{

	//int iRcvdBytes = 1000;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	
	if (iResult > 0)
	{
		printf("Bytes Received from client: %d  msg: %s \n", iResult, recvbuf);
		//iRcvdBytes = iResult;
	}
	else if (iResult == 0)
	{
		printf("Client closing connection \n");
	}
	else
	{
		printf("recv failed %d \n", WSAGetLastError());
		WSACleanup();
	}
	return recvbuf;

	// Echo it back
	//char *sendbuf = recvbuf;
	
}

// Lobby
void ClassTCPserver::serverMain()
{
	char buff[256];
	char option;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ZeroMemory(&hints, sizeof(hints));	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;  // Passive opens
	getaddrinfo(NULL, DEFAULT_PORT, &hints, &addressList);

	// Create a socket for connectong to the server
	ListenSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);
	BOOL optionValue = TRUE;
	int optionLength = sizeof(BOOL);
	setsockopt(ListenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optionValue, optionLength);

	// Set socket to listen on the port number provided (DEFAULT_PORT)
	bind(ListenSocket, addressList->ai_addr, (int)addressList->ai_addrlen);

	freeaddrinfo(addressList);

	listen(ListenSocket, SOMAXCONN);

	// Resolve the server address and port
	


	while (1)        // Loop until key pressed
	{
		// Waits for a message from the client
		printf("\r\nListening for connection...");
		messageRecieved();

		// Outputs on screen the options
		strcpy(buff, "Select an option: \n 1: Start Game \n 2: Exit Game");
		sendMessage(buff);

		// convert to integer
		strcpy(buff, messageRecieved());
		option = atoi(buff);

		if (option == 1)
		{
			// if 1 is pressed then send this message to the client
			strcpy(buff, "starting game \n");
			sendMessage(buff);
		
		}
		else if (option == 2)
		{
			// if 2 is pressed then send this message to the client
			strcpy(buff, "exiting lobby \n");
			sendMessage(buff);
			
		}
		
	}


	closesocket(ListenSocket);

	printf("\r\nHit key to exit");
	while (!_kbhit());

	WSACleanup();
}

