#include "TcpServer.h"

int main(){
	TcpServer server(8080);
	server.start();
	return 0;
}
