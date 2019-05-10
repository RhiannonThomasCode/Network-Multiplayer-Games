#pragma once
#include "Messages.h"
#include "PackUnpack.h"
class classUdp
{
	char restart = 'n';
	int game;
	int m1;
	char restart1 = 'n';
	int m2;
	int retrn;
	char restart2 = 'n';
	char restart3 = 'n';
	int hack;
	int v1;
	int n1;
	int n2;
	int n3;
	char restart4 = 'n';

public:
	classUdp();
	~classUdp();
	unsigned short packKey(Character *msg, unsigned char *buff);
	void MultiPlayer();
	int mainServerUDP();
};

