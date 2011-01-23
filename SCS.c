#include <stdio.h>
#include "messparser.h"
#include "messhandler.h"
#include <arpa/inet.h>

int main(int argc, char** argv)
{
	if(argc<2)
	{
		puts("first argument must be server`s ip adress");
		return 0;
	}
	int sock;
	struct sockaddr_in server_addr;	

	char msg_from_server[MAX_MSG_LENGTH];
	char msg_to_server[MAX_MSG_LENGTH];
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		puts("socket creation error");
		return 1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6667);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sock, (struct sockaddr*)& server_addr, sizeof(server_addr)) < 0)
	{
		puts("connection error");
		return 2;
	}
	msg_to_server[0] = SUPPORTED_PROTOCOL_VERSION; // Задаем версию протокола
	msg_to_server[1] = 0; // первый байт типа сбщ
	msg_to_server[2] = channel_setup; // второй байт типа сбщ
	msg_to_server[3] = 0; // первый байт размера сообщения
	msg_to_server[4] = 6; // второй байт размера сообщения
	msg_to_server[5] = 0; // первый байт типа параметра
	msg_to_server[6] = ECM_channel_id; // второй байт типа параметра
	msg_to_server[7] = 0;  //первый байт размера значения первого параметра
	msg_to_server[8] = 2; //второй байт размера значения первого параметра
	msg_to_server[9] = 0;
	msg_to_server[10] = 0x01; // второй байт значения первого параметра
	msg_to_server[11] = '\0';//орой байт значения первого параметра

	send(sock, msg_to_server, 11, 0); // отсылаем получившееся сообщение
	recv(sock, msg_from_server, 10, 0); 
	msg_to_server[0] = SUPPORTED_PROTOCOL_VERSION; // Задаем версию протокола
	msg_to_server[1] = 0; // первый байт типа сбщ
	msg_to_server[2] = channel_setup; // второй байт типа сбщ
	msg_to_server[3] = 0; // первый байт размера сообщения
	msg_to_server[4] = 6; // второй байт размера сообщения
	msg_to_server[5] = 0; // первый байт типа параметра
	msg_to_server[6] = ECM_channel_id; // второй байт типа параметра
	msg_to_server[7] = 0;  //первый байт размера значения первого параметра
	msg_to_server[8] = 2; //второй байт размера значения первого параметра
	msg_to_server[9] = 0;
	msg_to_server[10] = 0x01; // второй байт значения первого параметра
	msg_to_server[11] = '\0';//орой байт значения первого параметра

	send(sock, msg_to_server, 11, 0); // отсылаем получившееся сообщение
	printf("server: %s\n", msg_from_server); 
	
	close(sock);
	
	return 0;
}
