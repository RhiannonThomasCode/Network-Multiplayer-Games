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

	
	
		int			iRcvdBytes = 0;
		sockaddr_in	rx_addr;
		sockaddr_in sender_addr;
		int			iSenderAddrSize = sizeof(sender_addr);

		
		UdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (UdpSocket == INVALID_SOCKET)
		{
			printf("socket failed: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		
		rx_addr.sin_family = AF_INET;
		rx_addr.sin_port = htons((u_short)atol(DEFAULT_PORT));
		rx_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		iResult = bind(UdpSocket, (SOCKADDR *)&rx_addr, sizeof(rx_addr));
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed: %d\n", WSAGetLastError());
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}


		
		iResult = recvfrom(UdpSocket,
			recvbuf,
			recvbuflen,
			0,
			(SOCKADDR *)&sender_addr,
			&iSenderAddrSize);

		if (iResult == SOCKET_ERROR)
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes received: %d Msg=%s\n", iResult, recvbuf);

		
		iResult = sendto(UdpSocket,
			recvbuf,
			iResult,
			0,							// flags - we never need them so zero them
			(SOCKADDR *)&sender_addr,
			iSenderAddrSize);

		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(UdpSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);

		// cleanup
		closesocket(UdpSocket);
	

	while (!_kbhit());

	WSACleanup();

	return 0;
}


