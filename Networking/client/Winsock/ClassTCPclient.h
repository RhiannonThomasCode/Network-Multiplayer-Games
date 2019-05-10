#pragma once
class ClassTCPclient
{

	void sendMessage(char* message);
	char* messageRecieved();

public:
	ClassTCPclient();
	~ClassTCPclient();
	void clientMain();
};

