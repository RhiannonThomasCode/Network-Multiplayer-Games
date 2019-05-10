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

	const char   *serverName;

	if (argc < 2)
	{
		printf("usage: %s server-name\r\n", argv[0]);
		printf("Using loopback address instead..\r\n");
		serverName = "127.0.0.1";
	}
	else
	{
		serverName = argv[1];
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	while (!_kbhit())
	{
		SOCKET connectSocket = INVALID_SOCKET;
		char sendbuf[1000];      // Add stuff to

		sprintf(sendbuf, "%s %i", "this is a test", clock());

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		getaddrinfo(serverName, DEFAULT_PORT, &hints, &addressList);

		connectSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);

		//Active open
		connect(connectSocket, addressList->ai_addr, (int)addressList->ai_addrlen);

		freeaddrinfo(addressList);

		iResult = send(connectSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);

		printf("Bytes sent: &ld \n", iResult);

		shutdown(connectSocket, SD_SEND);

		// Block waiting to receive response from server
		iResult = recv(connectSocket, recvbuf, recvbuflen, 0);

		// YOU must handle the streaming of the bytes being received!! e.g create a buffer to store the until you get enough to unpack and use

		if (iResult > 0)
		{
			// iResult is the number od bytes received. Assumed NUL char is included in recvbuf to terminate the string
			printf("Bytes received: &d msg: %s \n", iResult, recvbuf);
		}
		else if (iResult == 0)
		{
			printf("Connection closed  by server");
		}
		else
		{
			printf("recv failed: &d \n", WSAGetLastError());
		}

		closesocket(connectSocket);
		
	}


	WSACleanup();
	while (_kbhit());  //Pause
	return 0;
}