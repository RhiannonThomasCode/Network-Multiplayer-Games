#include "pch.h"

#define _CRT_SECURE_NO_WARNINGS  1

#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "PackUnpack.h"
#include "Messages.h"

using namespace std;

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

unsigned short PackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
{

	PackShort(buf, preamb->code);
	PackShort(buf, preamb->length);

	return PRAMBLE_LENGTH;

}

unsigned short UnpackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
{

	UnpackShort(buf, preamb->code);
	UnpackShort(buf, preamb->length);

	return PRAMBLE_LENGTH;

}

unsigned short PackRubbishPreamble(MESSAGE_RUBBISH *preamb, unsigned char *buf)
{

	PackBytes(buf, preamb->username,20);
	PackLong(buf, preamb->longvalue);
	PackShort(buf, preamb->shortvalue);
	PackByte(buf, preamb->bytevalue);
	PackLong(buf, preamb->intValue);
	PackShort(buf, preamb->dataLength);
	PackBytes(buf, preamb->data,100);

	return MESSAGE_CODE_RUBBISH;

}

unsigned short UnpackRubbishPreamble(MESSAGE_RUBBISH *preamb, unsigned char *buf)
{
	unsigned char buffer[100];
	unsigned char *bufpt = buf;

	unsigned char *start = buffer;

	PackBytes(buf, preamb->username, 20);
	PackLong(buf, preamb->longvalue);
	PackShort(buf, preamb->shortvalue);
	PackByte(buf, preamb->bytevalue);
	PackLong(buf, preamb->intValue);
	PackShort(buf, preamb->dataLength);
	PackBytes(buf, preamb->data, 100);

	unsigned short length;
	 
	length = (unsigned short) (bufpt - start);

	return length;
}

unsigned short PackBulletPreamble(MESSAGE_BULLET_FIRED *preamb, unsigned char *buf)
{

	PackLong(buf, preamb->startX);
	PackLong(buf, preamb->startY);
	PackLong(buf, preamb->vectorX);
	PackLong(buf, preamb->vectorY);


	return MESSAGE_CODE_BULLET_FIRED;

}

unsigned short UnpackbulletPreamble(MESSAGE_BULLET_FIRED *preamb, unsigned char *buf)
{

	UnpackLong(buf, preamb->startX);
	UnpackLong(buf, preamb->startY);
	UnpackLong(buf, preamb->vectorX);
	UnpackLong(buf, preamb->vectorY);


	return MESSAGE_CODE_BULLET_FIRED;

}

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

	while (!_kbhit());

	WSACleanup();

	return 0;
}