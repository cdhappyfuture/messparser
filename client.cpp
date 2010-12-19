#include "connection.h"
#include <arpa/inet.h>

using namespace std;

const int MAX_MSG_LENGTH=2000;

int main(int argc, char** argv)
{
	if(argc<2)
	{
		cout << "first argument must be server`s ip adress\n";
		return 0;
	}
	cout << argc;
	int sock;
	struct sockaddr_in server_addr;	

	char msg_from_server[MAX_MSG_LENGTH];
	char msg_to_server[MAX_MSG_LENGTH];
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
	{
		cout << "socket creation error\n";
		return 1;
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(6667);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);

	if (connect(sock, (struct sockaddr*)& server_addr, sizeof(server_addr)) < 0)
	{
		cout << "connection error\n";
		return 2;
	}

	strcpy(msg_to_server,"lalalalcldldlldlldldlcdlcldcldcldldlcd");
	msg_to_server[3] = 0;
	msg_to_server[4] = 16;
	msg_to_server[7] = 0;
	msg_to_server[8] = 5;
	strcpy(&msg_to_server[9],"hello");	
	msg_to_server[16] = 0;
	msg_to_server[17] = 3;
	strcpy(&msg_to_server[18],"man");	

	send(sock, msg_to_server, 21, 0);
	recv(sock, msg_from_server, sizeof(msg_from_server), 0);
	cout << "server: " << msg_from_server;

/*	while(1)
	{
		cout << "me: ";
		cin >> msg_to_server;
		send(sock, msg_to_server, sizeof(msg_to_server), 0);
		recv(sock, msg_from_server, sizeof(msg_from_server), 0);
		cout << "server: " << msg_from_server;
		if(!strcmp(msg_from_server,"close_connection"))
			break;
	}
*/	while(1);
	cout << "\nconnection closed. Stopping program...\n";
	close(sock);
	
	return 0;
}
