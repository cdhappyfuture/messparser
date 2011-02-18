#include "myheaders.h"
#include "ECMG.h"
#include "messparser.h"
#include "messhandler.h"

int main(int argc, char** argv)
{
	pthread_t thread[MAX_SOCKETS];
	int listener;
	{ /* Запускаем сервер */
		listener = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(LISTEN_PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(listener, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
		{
			puts("bind port error!");
			return 2;
		}
		listen(listener, 1);
	}	
	puts("server started. waiting for clients");	
	int i=0;
	for (i = 0; i < MAX_SOCKETS; i++)
	{ /*Ждем клиента и запускаем его в отдельный поток выполнения */
		int client_sock = accept(listener,NULL,NULL);
		fcntl(client_sock, F_SETFL, O_NONBLOCK); // неблокирующий сокет
		pthread_create(&thread[i], NULL, &thread_fun, &client_sock);
	}
	int j;
	int _status;
	for (j = 0; j <= i; j++)	
		pthread_join(thread[j], _status);
	puts("Closing connection. Server stopped");
	close(listener);
	return 0;
}

void work_with_client(int sock)
{
	puts ("start work with client");
	Channel* channel = malloc(sizeof(Channel));
	Message* message;
	while(1)
	{
		/* если в результате обработки сообщения произошла ошибка или необходимо
		   закрыть TCP соединение и завершить поток выполнения,
		   то выходим из цикла */
		if (message = recv_and_deserialize(sock))
		{
			if (ECMG_messhandler(channel, message, sock)) 
			{
				puts("Закрываем TCP соединение");
				free_mes(message);
				break;
			}
		}
		else
		{
			puts("Ошибка при получении данных от клиента");
		//	free_mes(message);
			break;
		}
	}
	free(channel);
}

void* thread_fun(void* arg)
{
	int sock = *(int*) arg;
	work_with_client(sock);
}
