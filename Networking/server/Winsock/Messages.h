#pragma once

typedef struct MESSAGE_PREAMBLE
{
	unsigned short code;		//type of message
	unsigned short length;		//Length of message
};

#define PRAMBLE_LENGTH 4		//4 bytes (2 x unsigned short)

//code values
#define MESSAGE_CODE_BULLET_FIRED	1
#define MESSAGE_CODE_RUBBISH		2

typedef struct MESSAGE_BULLET_FIRED
{
	int startX;
	int startY;
	int vectorX;
	int vectorY;
};

typedef struct MESSAGE_RUBBISH
{
	unsigned char username[20];
	unsigned long longvalue;
	unsigned short shortvalue;
	unsigned char bytevalue;
	int intValue;

	//some variable length data
	unsigned short dataLength;
	unsigned char data[100];		//You dont reslly need this
};

struct Character
{
	char Character;
};