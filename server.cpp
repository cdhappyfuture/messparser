#include "myheader.h"
#include <boost/thread.hpp>
#include <stdint.h>

using namespace std;

const int MAX_MSG_LENGTH=2000;
const int MAX_SOCKETS=10;
const int LISTEN_PORT=6666;


class Connection
{
protected:
	int listener;
	sockaddr_in server_addr;
public:
	Connection()
	{	
		listener = socket(AF_INET, SOCK_STREAM, 0);
	}
	
	int _listen()
	{
		server_addr.sin:_family = AF_INET;
		server_addr.sin_port = htons(LISTEN_PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			cout << "bind port error";
			return 2;
		}	
		listen(listener, 1);
		return 0;
	}
	int get_sock()
	{
		return listener;
	}

	void close_conn()
	{
		close(listener);
	}
};

class Thread
{
protected:
	boost::thread thr;
public:
	Thread()
	{}
	void create(void (*func)(int,int),int s,int n)
	{
		thr = boost::thread(func,s,n);		
	}
	void waitfor()
	{
		thr.join();
	}
};

class Socket
{
protected:
	int sock;
public:
	void connect(int listener)
	{
		sock = accept(listener, NULL, NULL);
	}	
	int getsock()
	{
		return sock; 
	}
};

class Client: public Socket, public Thread
{
private:
	int number;
public:
	Client() 
	{}			
	int getnum()
	{ 
		return number; 
	}		
	void start(void (*func)(int,int),int listener,int i)
	{
		number = i; 
		connect(listener);
		create(func,sock,number);
	}
};

struct Parameter
{
	uint16_t parameter_type;
	uint16_t parameter_length;
	unsigned char* parameter_value;
};
	
struct Message
{
	unsigned char protocol_version;
	uint16_t message_type;
	uint16_t message_length;
	struct Parameter* parameter;
};

uint16_t char2_to_int(unsigned char* bytes)
{
	uint16_t var = bytes[0]*256 + bytes[1];
	return var;
}

void work_with_client(int,int);
struct Message parse(unsigned char*);

int main(int argc, char** argv)
{
	Client client[MAX_SOCKETS];
	Connection lstn;
	lstn._listen();
	cout << "server started. waiting for clients" << endl;	
	int i;
	for (i = 1; i <= MAX_SOCKETS; i++)	
		client[i-1].start(&work_with_client,lstn.get_sock(),i);
	for (int j = 1; j <= i; j++)	
		client[j-1].waitfor();
	cout << "Program exit\n";
	lstn.close_conn();
	
	return 0;
}

void work_with_client(int sock,int client_num)
{
	int bytes_read;
	char msg_from_client[MAX_MSG_LENGTH];
	char msg_to_client[MAX_MSG_LENGTH];
	strcpy(msg_to_client,"ok\n");
	while(1)
	{
		bytes_read = recv(sock, msg_from_client, sizeof(msg_from_client), 0);
		if(bytes_read <= 0)
		{
			close(sock);
			break;
		}
		if (!strcmp(msg_from_client,"close_connection"))
		{
			strcpy(msg_to_client,"close_connection");
			send(sock, msg_to_client, sizeof(msg_to_client), 0);
			cout << "client " << client_num << ": connection closed" << endl;
			close(sock);
			break;
		}	
		cout << "client " << client_num << ":" << msg_from_client << endl;
		send(sock, msg_to_client, sizeof(msg_to_client), 0);
	}
}

struct Message parse(unsigned char* input)
{
	struct Message* message = new Message;
	
	message.protocol_version = *input;
	input++;
	message.message_type = char2_to_int(input);
	input+=2;
	message.message_length = char2_to_int(input);
	input+=2;
	message.parameter = new Parameter[1024];
	unsigned char* ptr_first_param = input;
	int i=0;
	while (message.message_length >= input - ptr_first_param)
	{
		message.parameter[i].parameter_type = char2_to_int(input);	
		input+=2;
		message.parameter[i].parameter_length = char2_to_int(input);	
		input+=2;
		message.parameter[i].parameter_value = new char[message.parameter[i].parameter_length];
		memcpy(message.parameter[i].parameter_value, input, message.parameter[i].parameter_length);
		input += message.parameter[i].parameter_length;	
		i++;
	}	
	return message;
}
/*
	input[7] = 0;
	input[8] = 12; //а вот здесь количество символов из стоки которая ниже попадает в значение первого параметра  */
