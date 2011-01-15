#include "ECMG.h"
#include "myheaders.h"
#include "connection.h"
#include "messparser.h"
#include "messhandler.h"

int main(int argc, char** argv)
{
	Client client[MAX_SOCKETS];
	Connection lstn;
	lstn._listen();

	int sock = lstn.get_sock();
	std::cout << "server started. waiting for clients" << std::endl;	
	int i=0;
	for (i = 0; i < MAX_SOCKETS; i++)
	{
		client[i].start(&work_with_client,sock,i+1);
		i++;
	}
	for (int j = 0; j <= i; j++)	
		client[j].waitfor();
	std::cout << "Program exit" <<std::endl;
	lstn.close_conn();
	
	return 0;
}

void work_with_client(int sock,int client_num)
{
	std::cout << "work_with_client" << std::endl;
	char msg_to_client[MAX_MSG_LENGTH];
	Channel* channel = new Channel;
	Message* message;
	while(1)
	{
		message = parse(sock);
		if (messhandler(channel, message, sock)) 
		/* если в результате обработки сообщения необходимо
		закрыть TCP соединение и завершить поток выполнения,
		то выходим из цикла */
		{
			std::cout << "ошибка в handler" << std::endl;
			delete message;
			delete channel;
			return;
		}
		delete message;
	}
}
