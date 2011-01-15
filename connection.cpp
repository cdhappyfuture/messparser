#include "connection.h"

const int LISTEN_PORT = 6667;
Connection::Connection()
{
	listener = socket(AF_INET, SOCK_STREAM, 0);
}
int Connection::_listen()
{
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(LISTEN_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout << "bind port error!";
		return 2;
	}	
	listen(listener, 1);
	return 0;
}

int Connection::get_sock()
{
	return listener;
}

void Connection::close_conn()
{
	close(listener);
}


void Thread::create(void (*func)(int,int),int s,int n)	
{	
	thr = boost::thread(func,s,n);		
}
void Thread::waitfor()	
{
	thr.join();
}

void Socket::connect(int listener)
{
	sock = accept(listener, NULL, NULL);
}	
int Socket::getsock()
{
	return sock; 
}


int Client::getnum()
{ 
	return number; 
}		
void Client::start(void (*func)(int,int),int listener,int i)
{
	number = i; 
	connect(listener);
	create(func,sock,number);
}	
