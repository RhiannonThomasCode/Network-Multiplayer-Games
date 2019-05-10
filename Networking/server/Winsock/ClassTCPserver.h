#pragma once
class ClassTCPserver
{
	void sendMessage(char* message);
	char* messageRecieved();
	

public:
	void serverMain();
	ClassTCPserver();
	~ClassTCPserver();
};

