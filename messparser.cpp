#include "connection.h"

using namespace std;


const int MAX_MSG_LENGTH=2000;
const int MAX_SOCKETS=10;

struct Parameter
{
	uint16_t parameter_type;
	uint16_t parameter_length;
	char* parameter_value;
};
	
struct Message
{
	unsigned char protocol_version;
	uint16_t message_type;
	uint16_t message_length;
	struct Parameter* parameter;
};

uint16_t char2_to_int(char* bytes)
{
	uint16_t var = bytes[0]*256 + bytes[1];
	return var;
}

void work_with_client(int,int);
struct Message* parse(int);

int main(int argc, char** argv)
{
//	vector<Client> client(10);
	vector<Client> client;
//	Client init;
	Connection lstn;
	lstn._listen();

	int sock = lstn.get_sock();
	cout << "server started. waiting for clients" << endl;	
	int i=0;
//	while(1)
	for (i = 0; i < 10; i++)
	{
//		client.push_back(init);
		client[i].start(&work_with_client,sock,i+1);
		i++;
	}
	for (int j = 0; j <= i; j++)	
		client[j].waitfor();
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
		cout << message->parameter[0].parameter_value;
		strcpy(msg_to_client,"Message delivered");		
		send(sock, msg_to_client, sizeof(msg_to_client), 0);
	}
}

struct Message* parse(int input)
{
	struct Message* message = new Message;
	char buf[3];
	recv(input,buf,1,0);
	message->protocol_version = (unsigned int)*buf;
	recv(input,buf,2,0);
	message->message_type = char2_to_int(buf);
	recv(input,buf,2,0);
	message->message_length = char2_to_int(buf);
	message->parameter = new Parameter[500];
	int i = 0;
	int mes_value_curr_size = 0;
	while (message->message_length >= mes_value_curr_size)
	{
		recv(input,buf,2,0);
		message->parameter[i].parameter_type = char2_to_int(buf);	
		recv(input,buf,2,0);
		message->parameter[i].parameter_length = char2_to_int(buf);	
		message->parameter[i].parameter_value = new char[message->parameter[i].parameter_length];
		recv(input, message->parameter[i].parameter_value, message->parameter[i].parameter_length, 0);
		i++;
		mes_value_curr_size += 4 + message->parameter[i].parameter_length;
	}	
	return message;
}
