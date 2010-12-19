#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <stdint.h>
#include <vector>
#include <boost/thread.hpp>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>

const int LISTEN_PORT = 6666;
	
class Connection
{
protected:
	int listener;
	sockaddr_in server_addr;
public:
	int _listen();
	int get_sock();
	void close_conn();
};

class Thread
{
protected:
	boost::thread thr;
public:
	Thread() {}
	void create(void (*func)(int,int),int s,int n);
	void waitfor();
};

class Socket
{
protected:
	int sock;
public:
	void connect(int listener);
	int getsock();
};

class Client: public Socket, public Thread
{
private:
	int number;
public:
	Client() {}
	int getnum();
	void start(void (*func)(int,int),int listener,int i);
};

#endif
