#include "myheader.h"
#include <boost/thread.hpp>
#include <stdint.h>

using namespace std;


const int MAX_MSG_LENGTH=2000;
const int MAX_SOCKETS=10;
const int LISTEN_PORT=6666;
//commit
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
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(LISTEN_PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			cout << "bind port error!";
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
		Message* message = new Message;
		message = parse(sock);
		strcpy(msg_to_client,"Message delivered");		
		send(sock, msg_to_client, sizeof(msg_to_client), 0);
	}
}

struct Message* parse(int input)
{
	struct Message* message = new Message;
	char buf[3];
	recv(input,buf,1,0);
	message->protocol_version = (unsigned int)buf;
	recv(input,buf,2,0);
	message->message_type = char2_to_int(buf);
	recv(input,buf,2,0);
	message->message_length = char2_to_int(buf);
	message->parameter = new Parameter[500];
	unsigned char* ptr_first_param = input;
	int i=0;
	while (message->message_length >= input - ptr_first_param)
	{
		recv(input,buf,2,0);
		message->parameter[i].parameter_type = char2_to_int(buf);	
		recv(input,buf,2,0);
		message->parameter[i].parameter_length = char2_to_int(buf);	
		message->parameter[i].parameter_value = new char[message->parameter[i].parameter_length];
		recv(input, message->parameter[i].parameter_value, message->parameter[i].parameter_length, 0);
		i++;
	}	
	return message;
}
/*
	input[7] = 0;
	Mreinput[8] = 12; //а вот здесь количество символов из стоки которая ниже попадает в значение первого параметра  */
