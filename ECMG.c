#include "ECMG.h"
#include "messparser.h"
#include "messhandler.h"

int main(int argc, char** argv)
{
	pthread_t thread[MAX_SOCKETS];
	int listener;
	{ /* Запускаем сервер */
		listener = socket(AF_INET, SOCK_STREAM, 0);
		sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(LISTEN_PORT);
		server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		if(bind(listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
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
		pthread_create(&thread[i], NULL, &thread_fun, &client_sock)
	}
	for (int j = 0; j <= i; j++)	
		pthread_join(thread[j]);
	puts("Closing connection. Server stopped");
	close(listener);
	return 0;
}

void work_with_client(int sock)
{
	Channel* channel = malloc(sizeof(Channel));
	Message* message;
	while(1)
	{
		message = parse(sock);
		if (messhandler(channel, message, sock)) 
		/* если в результате обработки сообщения необходимо
		закрыть TCP соединение и завершить поток выполнения,
		то выходим из цикла */
		{
			puts("ошибка в handler");
			free(message);
			free(channel);
			return;
		}
		free(message);
	`	free(channel);
	}
}

void* thread_fun(void* arg)
{
	int sock = *(int*) arg;
	if (work_with_client(sock))
		puts("error in thread");
}
