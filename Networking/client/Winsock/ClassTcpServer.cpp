#include "pch.h"

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

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		iResult;
	int		recvbuflen = DEFAULT_BUFLEN;

	struct addrinfo *addressList = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET listenSocket = INVALID_SOCKET;
	SOCKET clientSocket = INVALID_SOCKET;

	int iRcvdBytes = 0;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;  // Passive opens
	getaddrinfo(NULL, DEFAULT_PORT, &hints, &addressList);

	// Create a socket for connectong to the server
	listenSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);

	BOOL optionValue = TRUE;
	int optionLength = sizeof(BOOL);
	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&optionValue, optionLength);

	// Set socket to listen on the port number provided (DEFAULT_PORT)
	bind(listenSocket, addressList->ai_addr, (int)addressList->ai_addrlen);

	freeaddrinfo(addressList);

	listen(listenSocket, SOMAXCONN);

	while (!_kbhit())
	{
		printf("Listening for incoming connections \n");
		
		// Block waiting for somebody to talk to me!!
		clientSocket = accept(listenSocket, NULL, NULL);

		iResult - recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf("Bytes Received from client: %d  msg: %s", iResult, recvbuf);
				iRcvdBytes = iResult;
		}
		else if (iResult == 0)
		{
			printf("Client closing connection \n");
		}
		else
		{
			printf("recv failed %d \n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Echo it back
		char *sendbuf = recvbuf;
		iResult = send(clientSocket, sendbuf, iRcvdBytes, 0);

		shutdown(clientSocket, SD_SEND);  // Forces send
		closesocket(clientSocket);
	}

	closesocket(listenSocket);  // Server no longer required

	WSACleanup();
	while (!_kbhit());
	return 0;
}