#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <string>
#include <netinet/in.h>

class TcpServer{
public:
	explicit TcpServer(int port);
	~TcpServer();
	void start();
	void stop();

private:
	int server_fd;
	int port;
	bool running;
	sockaddr_in server_addr;

	void handleClient(int client_fd);
};

#endif
