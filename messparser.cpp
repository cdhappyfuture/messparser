#include "connection.h"

using namespace std;


const int MAX_MSG_LENGTH=2000;
const int MAX_SOCKETS=10;

struct Parameter
{
	uint16_t type;
	uint16_t length;
	char* value;
};
	
struct Message
{
	unsigned char protocol_version;
	uint16_t type;
	uint16_t length;
	vector<Parameter> parameter;
};

uint16_t char2_to_int(char* bytes)
{
	uint16_t var = (unsigned int)bytes[0]*256 + (unsigned int)bytes[1];
	return var;
}

void work_with_client(int,int);
struct Message* parse(int);

int main(int argc, char** argv)
{
	Client client[MAX_SOCKETS];
	Connection lstn;
	lstn._listen();

	int sock = lstn.get_sock();
	cout << "server started. waiting for clients" << endl;	
	int i=0;
	for (i = 0; i < MAX_SOCKETS; i++)
	{
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
	char msg_to_client[MAX_MSG_LENGTH];
		Message* message = new Message;
		message = parse(sock);
		for (int i = 0; i < message->parameter.size(); i++)
			cout << message->parameter[i].value << " ";
		cout << endl;
		strcpy(msg_to_client,"Message delivered\n");
		send(sock, msg_to_client, sizeof(msg_to_client), 0);
}

struct Message* parse(int input)
{
	struct Message* message = new Message;
	char buf[3];
	recv(input,buf,1,0);
	message->protocol_version = (unsigned int)*buf;
	cout << "Protocol version: " << *buf <<  endl;
	recv(input,buf,2,0);
	cout << "Message type: " << buf[0] << buf[1] <<  endl;
	message->type = char2_to_int(buf);
	recv(input,buf,2,0);
	cout << "Message length: " << char2_to_int(buf) <<  endl;
	message->length = char2_to_int(buf);
	int i = 0;
	int mes_value_curr_size = 0;
	while (message->length >= mes_value_curr_size + 4)
	{
		Parameter param;	
		recv(input,buf,2,0);
		param.type = char2_to_int(buf);	
		recv(input,buf,2,0);
		param.length = char2_to_int(buf);
		mes_value_curr_size += 4;
		if (param.length + mes_value_curr_size > message->length)
			break;
		param.value = new char[param.length];
		recv(input, param.value, param.length, 0);
		message->parameter.push_back(param);
		mes_value_curr_size += message->parameter[i].length;
		i++;
	}
	return message;
}
