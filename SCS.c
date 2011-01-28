#include <stdio.h>
#include "myheaders.h"
#include "messparser.h"
#include "messhandler.h"
#include "messages.h"
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	/*if(argc<2)
	{
		puts("first argument must be server`s ip adress");
		return 0;
	}*/
	int sock;
	struct sockaddr_in server_addr;	

	char msg_from_server[MAX_MSG_LENGTH];
	char* msg_to_server;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		puts("socket creation error");
		return 1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6667);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr*)& server_addr, sizeof(server_addr)) < 0)
	{
		puts("connection error");
		return 2;
	}
	
	f_channel_setup(sock);
	printf("server: %s\n", msg_from_server); 
	while(1){}	
//	close(sock);
	
	return 0;
}

