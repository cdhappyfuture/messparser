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

	Message* message = malloc(sizeof(Message));
	message->parameter = calloc(2, sizeof(Parameter*));
	message->parameter[0] = malloc(sizeof(Parameter));
	message->protocol_version = SUPPORTED_PROTOCOL_VERSION;
	message->type = htons(channel_setup);
	message->params = 1;
	puts("Переходим к параметру");
	message->parameter[0]->type = htons(ECM_channel_id);
	message->parameter[0]->length = htons(2);
	puts("Переходим к значению параметра");
	message->parameter[0]->value = malloc(message->parameter[0]->length + 1);
	puts("Выделили память");
	uint16_t param_value = 1;
	param_value = htons(param_value);
	printf("Param_value for socket = %i\n", param_value);
	puts("Пытаемся записать значение");
	memcpy((void*)message->parameter[0]->value, (void*)&param_value, 2);
	puts("Задаем длину сбщ");
	message->length = htons(sizeof(message->parameter[0]->type) + sizeof(message->parameter[0]->length) +
		ntohs(message->parameter[0]->length));
	printf("meslengthH = %i\n", message->length);
	printf("strlen %i", strlen(message->parameter[0]->value));
	uint16_t foo;
	memcpy((void*)&foo, (void*)message->parameter[0]->value, 2);
	printf("Param_value after socket= %i\n", foo);
	foo = ntohs(foo);
	printf("Param_value for host = %i\n", foo);
	msg_to_server = unparse(message);
	printf("message->length for socket = %i", ntohs(message->length));
	send(sock, msg_to_server, sizeof(message->protocol_version) +
		sizeof(message->type) + sizeof(message->length) + ntohs(message->length), 0); // отсылаем получившееся сообщение
	recv(sock, msg_from_server, 10, 0); 
	printf("server: %s\n", msg_from_server); 
	
	close(sock);
	
	return 0;
}
