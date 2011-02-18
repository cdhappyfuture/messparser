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
	Message* message = recv_and_deserialize(sock);
	if (message->type == channel_status && char2_to_int(message->parameter[0]->value) == 1)
	{
		puts("Channel 1 setup success");
		f_channel_test(sock);
		if (message->type == channel_status && char2_to_int(message->parameter[0]->value) == 1)
		/*{
			f_stream_setup(sock, 7, 1);
			f_stream_close_request(sock, 1);
		}
		else*/
		{
			puts("Channel error!! Sending channel_close");
			f_channel_close(sock);
		}
	}
	else
	{
		puts("Channel 1 setup fail");
		puts("Sending channel_close...");
		f_channel_error(sock, unrecoverable_error);
	}
	puts("Shutting down...");
	close(sock);
	
	return 0;
}

