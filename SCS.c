#include <stdio.h>
#include "myheaders.h"
#include "messparser.h"
#include "messhandler.h"
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
	recv(sock, msg_from_server, 10, 0); 
	printf("server: %s\n", msg_from_server); 
	
	close(sock);
	
	return 0;
}

void f_channel_setup(int sock)
{		
	Message* message = malloc(sizeof(Message));
	message->type = channel_setup;
	message->params = 1;
	alloc_params(message);
	set_param(message, 0, ECM_channel_id, 0x0002, 0x0002);
	setup_and_send(sock, message);
	free_mes(message);	
}

void alloc_params(Message* message)
{
	message->parameter = calloc(message->params, sizeof(Parameter*));
	int i;
	for (i = 0; i < message->params; i++)
		message->parameter[i] = malloc(sizeof(Parameter));
}	

void setup_and_send(int sock, Message* message)
{	
	message->protocol_version = SUPPORTED_PROTOCOL_VERSION;
	message->type = htons(message->type);
	int i;
	for (i = 0; i < message->params; i++ )
		message->length += sizeof(message->parameter[i]->type) + sizeof(message->parameter[i]->length) +	
			ntohs(message->parameter[i]->length);
	message->length = htons(message->length);
	char* msg_to_server = unparse(message);
	send(sock, msg_to_server, sizeof(message->protocol_version) + sizeof(message->type) + 
		sizeof(message->length) + ntohs(message->length), 0); // отсылаем получившееся сообщение
}
void set_param(Message* message, int i, uint16_t type, uint16_t len, uint16_t value)
{	
	message->parameter[i]->type = htons(type);
	message->parameter[i]->value = malloc(message->parameter[i]->length + 1);
	message->parameter[i]->length = htons(len);
	value = htons(value);
	memcpy((void*)message->parameter[i]->value, (void*)&value, len);
}
