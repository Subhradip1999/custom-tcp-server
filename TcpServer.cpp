#include "TcpServer.h"
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <csignal>
#include <thread>
#include <sstream>
#include <fstream>

TcpServer* globalServerInstance = nullptr;

void signalHandler(int signum) {
    if (globalServerInstance) {
        globalServerInstance->stop();
    }
    exit(0);
}

TcpServer::TcpServer(int port) : port(port), running(true) {
    globalServerInstance = this;

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
	
	int opt = 1;
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	//Bind socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);
	
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Binding Failed");
		exit(EXIT_FAILURE);
	}
	
	//Listen for connection
	if(listen(server_fd, 5)<0){
		perror("Listening failed");
		exit(EXIT_FAILURE);
	}
	
	std::cout << "Server started on port " << port << std::endl;
}

TcpServer::~TcpServer(){
		close(server_fd);
		std::cout << "Server stopped.\n";
		
}

void TcpServer::start() {
		signal(SIGINT, signalHandler);
		
		
		while(running){
			sockaddr_in client_addr;
			socklen_t client_len = sizeof(client_addr);
			int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
			
			if (client_fd<0) {
				perror("Accept Failed");
				continue;
			}
			
			std::cout << "Client Connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;
			std::thread(&TcpServer::handleClient, this, client_fd).detach(); // separate threads for handling multiple client requests.
			//close(client_fd);
		}
}

void TcpServer::stop() {
	running = false;
	close(server_fd);
	std::cout << "Server shutting down...\n";
}

void TcpServer::handleClient(int client_fd) {
	char buffer[1024] = {0};
	read(client_fd, buffer, sizeof(buffer));
  
  std::string request(buffer);
  std::istringstream request_stream(request);
  std::string method, path, version;
  
  request_stream >> method >> path >> version; //extract HTTP method, path and version.
  std::cout << "Method: " << method << ", Path: " << path << " and, Version: " << version << std::endl;
  
  std::string filename = "public/index.html";
  if (request.find("GET / ") == std::string::npos) {
     size_t pos1 = request.find("GET / ") + 5;
     size_t pos2 = request.find(" ", pos1);
     filename = "public/" + request.substr(pos1, pos2 - pos1);
  }
  
  std::ifstream file(filename, std::ios::binary);
  if (file) {
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string content = ss.str();
    
    //determine content type
    std::string contentType = "text/html";
    if (filename.find(".css") != std::string::npos) contentType = "text/css";
    else if (filename.find(".js") != std::string::npos) contentType = "application/javascript";
    
    //send http response
    std::string response = "HTTP/1.1 200 OK\r\nontent-Type: " + contentType + "\r\nContent-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;
    send(client_fd, response.c_str(), response.length(), 0);
    
  } else {
    std::string notFoundResponse = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(client_fd, notFoundResponse.c_str(), notFoundResponse.size(), 0);
  }
	/**
   //simple http text response
	std::string response;
  if (method == "GET" && path == "/") {
     response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello client, I am C++ HTTP server, listening to you...!";
  } else {
     response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nPage Not Found...!  ";
  }
  
	send(client_fd, response.c_str(), response.length(), 0);
  **/
 
  close(client_fd);
}

