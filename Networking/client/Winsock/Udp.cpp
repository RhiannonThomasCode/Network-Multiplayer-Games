// Winsock.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		iResult;
	int		recvbuflen = DEFAULT_BUFLEN;
	SOCKET	UdpSocket = INVALID_SOCKET;

	struct addrinfo *addressList = NULL;
	struct addrinfo hints;

	

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	

		const char *sendbuf = "this is a test";

		
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		
		iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &addressList);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		
		if (addressList != NULL)
		{

		
			UdpSocket = socket(addressList->ai_family,
				addressList->ai_socktype,
				addressList->ai_protocol);

			
			if (UdpSocket == INVALID_SOCKET)
			{
				printf("Error at socket(): %ld\n", WSAGetLastError());
				freeaddrinfo(addressList);
				WSACleanup();
				return 1;
			}

			
		}

		
		iResult = sendto(UdpSocket,
			sendbuf,
			(int)strlen(sendbuf) + 1,
			0,							// flags - we never need them so zero them
			addressList->ai_addr,
			(int)addressList->ai_addrlen);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			freeaddrinfo(addressList);
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);

		// For fun!!, lets find out the dynamic port number used.
		//
		sockaddr_in client_addr;
		int			iClientAddrLen = sizeof(client_addr);
		iResult = getsockname(UdpSocket, (SOCKADDR *)&client_addr, &iClientAddrLen);
		if (iResult == SOCKET_ERROR)
		{
			printf("getsockname failed: %d\n", WSAGetLastError());
			freeaddrinfo(addressList);
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(addressList);

		// Receive a datagram packet from the socket.
		//
		iResult = recvfrom(UdpSocket,
			recvbuf,
			recvbuflen,
			0,			// flags we don't need, so set to zero
			NULL,
			NULL);

		if (iResult == SOCKET_ERROR)
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}
		printf("Bytes received: %d Msg=%s\n", iResult, recvbuf);

		// cleanup
		closesocket(UdpSocket);
	
	

	while (!_kbhit());

	WSACleanup();

	return 0;
}


