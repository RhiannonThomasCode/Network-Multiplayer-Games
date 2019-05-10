#include "pch.h"
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

classUdp::classUdp()
{
}


classUdp::~classUdp()
{
}


unsigned short classUdp::packKey(Character *msg, unsigned char *buff) {
	unsigned short msglen;
	unsigned char *lenptr;

	PackByte(buff, msg->Character);

	msglen = sizeof(msg->Character);

	return msglen;
}

// Calculates the data
void classUdp::MultiPlayer() {

	/*int i = 1;

	do {
		system("cls");
		restart1 = 'n';
		cout << "============================================================" << endl;
		cout << "           number guessing game(multiple player)" << endl;
		cout << "============================================================\n\n" << endl;
		cout << "Player 1" << endl;
		cout << "-please enter a number betwen 0-100: ";
		cin >> n1;

		if (n1 > 100) {
			cout << "\n\n***********************************************" << endl;
			cout << "ERROR\nThe number is to big please re-enter" << endl;
			cout << "\n\n***********************************************" << endl;
			system("pause");
			system("cls");
			restart1 = 'y';
		}


	} while (restart1 == 'y');

	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;
	cout << "player 2\n" << endl;
	cout << "type in the number you want to guess,type enter to start\n" << endl;
	cout << "               ======GOOD LUCK======" << endl;
	system("pause");

	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;

	cout << "               ===gamestart===\n\n" << endl;
	do {
		restart2 = 'n';
		cout << i;
		cout << ": ";
		cin >> n2;
		if (i == v1) {
			cout << "\nsorry, you are out of attempts\n\n\n" << endl;
			system("pause");
		}

		else if (n2 == n1) {
			cout << "You win!" << endl;
		}
		else if (n2 < n1) {
			cout << "bigger" << endl;
			i++;
			restart2 = 'y';
		}
		else if (n2 > n1) {
			cout << "smaller" << endl;
			i++;
			restart2 = 'y';
		}


	} while (restart2 == 'y');
	int x = 1;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                                                                        loading final result:\n" << endl;
	cout << "                                                                     ";
	while (x < 26) {
		cout << "=";
		Sleep(0100);
		x++;
	}
	system("cls");
	cout << "============================================================" << endl;
	cout << "           number guessing game(multiple player)" << endl;
	cout << "============================================================\n\n" << endl;

	cout << "The final result is: \n\n" << endl;
	cout << "The player 1's number is " << n1 << endl;
	cout << "number of guessing: " << i << endl;
	system("pause");*/



}


// Server UDP
int classUdp::mainServerUDP()
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


