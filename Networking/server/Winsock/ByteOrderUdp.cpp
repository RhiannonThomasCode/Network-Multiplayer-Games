#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "Packing.h"

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

	if (argc < 2)
	{
		printf("usage: %s -client server-name\r\n", argv[0]);
		printf("usage: %s -server\r\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	// The WSAStartup function is called to initiate use of WS2_32.lib.
	// The WSADATA structure contains information about the Windows Sockets 
	// implementation. The MAKEWORD(2,2) parameter of WSAStartup makes a 
	// request for the version of Winsock on the system, and sets the passed 
	// version as the highest version of Windows Sockets support that the 
	// caller can use.
	//
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	if (strcmp(argv[1], "-client") == 0)
	{
		MSG_STRUCT msg;
		int i;
		unsigned char sendbuf[1000];
		unsigned short buflen;

		// Validate the parameters
		if (argc != 3)
		{
			printf("usage: %s -client server-name\n", argv[0]);
			return 1;
		}

		// Somewhere in the application you create a message
		// to send and fill it with the data.
		msg.code = MSGCODE;
		msg.length = 0;                         // Fill this in later when we pack message.
		strcpy((char*)msg.username, "Mike");
		msg.longValue = 0x12345678;
		msg.shortValue = 0x1234;
		msg.byteValue = 0x12;
		msg.intValue = -22;
		for (i = 0; i < 60; i++)
		{
			msg.data[i] = i; // 60 bytes of rubbish
		}
		msg.datalen = 60;

		// Now pack the message structure into the send buffer.
		buflen = PackMsg(&msg, sendbuf);
		printf("Message being sent: code:%i\n usr:%s\n l:0x%x s:0x%x b:0x%x i:%i\n datalen:%i\n",
			msg.code,
			msg.username,
			msg.longValue, msg.shortValue, msg.byteValue, msg.intValue,
			msg.datalen);

		// ****************************************
		// After initialization, a SOCKET object must be instantiated
		// ****************************************

		// Call the getaddrinfo function requesting the IP address for the 
		// server name passed on the command line. For this application, 
		// the Internet address family is unspecified so that either an 
		// IPv6 or IPv4 address can be returned. The application does request 
		// the socket type to be a datagram socket for the UDP protocol.
		// - SOCK_DGRAM Supports datagrams, which are connectionless, 
		//   unreliable buffers of a fixed (typically small)maximum length. 
		//   Uses the User Datagram Protocol (UDP). 
		//
		// The getaddrinfo function provides protocol-independent translation 
		// from an ANSI host name to an address.
		//
		// int WSAAPI getaddrinfo(	const char FAR* nodename, 
		//							const char FAR* servname,
		//							const struct addrinfo FAR* hints, 
		//							struct addrinfo FAR** res);
		// nodename = A pointer to a NULL-terminated ANSI string that contains 
		// a host (node) name or a numeric host address string.
		// For the Internet protocol, the numeric host address string is a 
		// dotted-decimal IPv4 address or an IPv6 hex address.
		// servname = A pointer to a NULL-terminated ANSI string that contains 
		// either a service name or port number represented as a string
		// hints = A pointer to an addrinfo structure that provides hints 
		// about the type of socket the caller supports.
		// res = Output. A pointer to a linked list of one or more addrinfo structures 
		// that contains response information about the host. 
		//
		// The getaddrinfo function is used to determine the values in the 
		// sockaddr structure:
		//
		// - AF_UNSPEC (i.e. unspecified) is used so either IPV6 or IPv4 
		// addresses are possible for Internet address family. 
		// - the IP address of the server that the client will connect to. 
		// - 27015 is the port number associated with the server that the client will connect to. 
		//
		// Returns a linked list of addresses, the memory for which must be
		// freed by calling freeaddrinfo() when finished with.
		//
		// You may directly set an IP address by creating a sockaddr_in 
		// object clientService and setting its values e.g.
		// sockaddr_in txAddr;	// Internet (_in i.e. IP) Address for socket
		// hostent* localHost;
		// char* localIP;
		// localHost = gethostbyname("");
		// localIP = inet_ntoa (*(struct in_addr *)*localHost->h_addr_list);
		// txAddr.sin_family = AF_INET;
		// txAddr.sin_addr.s_addr = inet_addr(localIP);
		// OR just this txAddr.sin_addr.s_addr = inet_addr("127.0.0.1");		
		// txAddr.sin_port = htons(5150);
		// sendto(SendSocket, SendBuf, BufLen, 0, 
		//			(SOCKADDR *) &txAddr, sizeof(txAddr));
		//
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;

		// Using the hostname(or IP address) typed in by the user, use DNS to
		// Resolve the server address, the resultant address should use
		// the port number we provide.
		//
		iResult = getaddrinfo(argv[2], DEFAULT_PORT, &hints, &addressList);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// If we have a valid address then send a message to it.
		//
		if (addressList != NULL)
		{

			// Call the socket function and return its value to the ClientSocket 
			// variable. For this application, use the Internet address family, 
			// streaming sockets, and the TCP/IP protocol returned by the call to 
			// getaddrinfo.
			//

			// Create a SOCKET for connecting to server
			UdpSocket = socket(addressList->ai_family,
				addressList->ai_socktype,
				addressList->ai_protocol);

			// Check for errors to ensure that the socket is a valid socket.
			// Error detection is a key part of successful networking code. 
			// If the socket call fails, it returns INVALID_SOCKET. The if 
			// statement in the previous code is used to catch any errors that may 
			// have occurred while creating the socket. 
			// WSAGetLastError returns an error number associated with the last error 
			// that occurred.
			// WSACleanup is used to terminate the use of the WS2_32 DLL.
			//
			if (UdpSocket == INVALID_SOCKET)
			{
				printf("Error at socket(): %ld\n", WSAGetLastError());
				freeaddrinfo(addressList);
				WSACleanup();
				return 1;
			}

			// Note: We could use the connect() function for UDP. 
			// For a connectionless socket (for example, type SOCK_DGRAM), 
			// the operation performed by connect is merely to establish a 
			// default destination address that can be used on subsequent 
			// send/ WSASend and recv/ WSARecv calls.
			// Any datagrams received from an address other than the 
			// destination address specified will be discarded. If the address
			// member of the structure specified by name is all zeroes, 
			// the socket will be disconnected. Then, the default remote
			// address will be indeterminate, so send/ WSASend and recv/ WSARecv
			// calls will return the error code WSAENOTCONN. However, 
			// sendto/ WSASendTo and recvfrom/ WSARecvFrom can still be used. 
			// The default destination can be changed by simply calling 
			// connect again, even if the socket is already connected. 
			// Any datagrams queued for receipt are discarded if name is 
			// different from the previous connect.
			// Call the connect function, passing the created socket and the 
			// sockaddr structure as parameters. Check for general errors.
			//
			//iResult = connect( UdpSocket, addressList->ai_addr, (int)addressList->ai_addrlen);
			//if (iResult == SOCKET_ERROR) 
			//{
			//	closesocket(UdpSocket);
			//	UdpSocket = INVALID_SOCKET;
			//	printf("Unable to set default address!\n");
			//	freeaddrinfo(addressList);
			//	WSACleanup();
			//	return 1;
			//}
		}

		// Note: We don't need to bind the client socket because
		// we want it to create a dynamic port number, since the
		// well-known port numbers are only be used by the server.
		// The client always sends to a well known port number, the
		// server sends back the results to the dynamic port number
		// used by the client.
		// The dynamic port number is bound to the socket during a
		// call to sendto().
		//

		// Note: If you use connect() to setup a default address, you can
		// use the send and recv functions.
		// Both these functions return an integer value of the 
		// number of bytes sent or received, respectively, or an error. 


		// Send an initial buffer to a specific destination.
		// The to parameter can be any valid address in the socket's 
		// address family, including a broadcast or any multicast address. 
		// To send to a broadcast address, an application must have used
		// setsockopt with SO_BROADCAST enabled. Otherwise, sendto will 
		// fail with the error code WSAEACCES. For TCP/IP, an application 
		// can send to any multicast address (without becoming a group member).
		//
		printf("Sending %i bytes\n", buflen);
		iResult = sendto(UdpSocket,
			(char *)sendbuf,
			(int)buflen,
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

		freeaddrinfo(addressList);

		// Receive a datagram packet from the socket.
		// Receive ack from server that it got our message.
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
	}
	else
	{
		MSG_STRUCT  msg;
		int			iRcvdBytes = 0;
		sockaddr_in	rx_addr;
		sockaddr_in sender_addr;
		int			iSenderAddrSize = sizeof(sender_addr);

		// Create a UDP datagram socket.
		// - AF_INET is used to specify the IPv4 address family. 
		// - SOCK_DGRAM Supports datagrams, which are connectionless, 
		//   unreliable buffers of a fixed (typically small)maximum length. 
		//   Uses the User Datagram Protocol (UDP). 
		//
		UdpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (UdpSocket == INVALID_SOCKET)
		{
			printf("socket failed: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Bind (i.e. associate to local address) the socket to ANY IP address 
		// but a specified "known" port number.
		// i.e. we can receive on any of our ethernet cards but the socket
		// will only receive those for the UDP port we specify.
		// Note: You can use a specific address if you wish.
		// e.g rx_addr.sin_addr.s_addr = inet_addr("194.10.10.1");		
		//
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


		// Receive datagram packet from socket.
		// Provide a sender address and length field, so that
		// we can get the address of the sender of this packet.
		// Returns number of bytes received or error.
		//
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

		// Unpack the buffer into the message structure.
		// At this point the application would process
		// the message data.
		UnpackMsg(&msg, (unsigned char*)recvbuf);
		printf("Message received: %d bytes code:%i\n usr:%s\n l:0x%x s:0x%x b:0x%x i:%i\n datalen:%i\n",
			iResult, msg.code,
			msg.username,
			msg.longValue, msg.shortValue, msg.byteValue, msg.intValue,
			msg.datalen);

		// Send back a simple Ack that we got the message.
		strcpy(recvbuf, "Got message");
		iResult = strlen(recvbuf) + 1;

		// Send the received buffer straight back to the
		// sender.
		//
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

		printf("Response Sent: %ld Msg=%s\n", iResult, recvbuf);

		// cleanup
		closesocket(UdpSocket);
	}

	while (!_kbhit());

	WSACleanup();

	return 0;
}