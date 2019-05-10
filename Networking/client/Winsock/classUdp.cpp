#include "pch.h"
#include "ClassGame.h"
#include "classUdp.h"

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <cstdlib>
#include <time.h>
#include <windows.h>


using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

ClassGame guess;


classUdp::classUdp()
{
}


classUdp::~classUdp()
{
}

// Displays the data
void classUdp::MultiPlayer()
{
		/*system("color 0A");
		int i = 1;
		system("color 0A");
		restart = 'n';
		system("cls");
		cout << "============================================================" << endl;
		cout << "           number guessing game(multiple player)" << endl;
		cout << "============================================================\n\n" << endl;
		cout << "                   ===game rules===\n---The first player needs to type in a number bewten 1-100\n---The second player needs to guess the number\n\n" << endl;
		system("pause");

		system("cls");
		cout << "============================================================" << endl;
		cout << "           number guessing game(multiple player)" << endl;
		cout << "============================================================\n\n" << endl;
		cout << "please enter the maximum of attempts: ";
		cin >> v1;*/
}
// Client UDP
unsigned short classUdp::packKey(Character *msg, unsigned char *buff) 
{
	unsigned short msglen;
	unsigned char *lenptr;

	PackByte(buff, msg->Character);

	msglen = sizeof(msg->Character);

	return msglen;
}

void classUdp::unPackKey(Character *msg, unsigned char* buff)
{
	UnpackByte(buff,msg->Character)
}

int classUdp::mainClientUDP()
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


