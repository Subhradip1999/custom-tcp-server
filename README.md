# custom-tcp-server
A TCP/IP server for serving HTTP responses, using C++.
This server is developed solely for educational purposes and should not be used as a production-level server. It is a very basic server, and it lacks some modern-day features, like SSL certification, which is  mandatory for a server to become a production-level server.
This is developed to be hosted on Raspberry Pi Zero W board, flashed with Raspberry Pi OS 32-bit.
This TCP HTTP server will work on any Linux-based system and even Windows with WSL (Windows Subsystem for Linux) or MacOS, not just Raspberry Pi. Also, this server can serve static files like CSS and JavaScript as well, in such cases, the folder structure should be:

```
## Project Structure

/tcp_server_project/
│── server.cpp
│── TcpServer.cpp
│── TcpServer.h
│── public/
│   ├── index.html
│   ├── styles.css
│   ├── script.js
│   ├── images/
│   │   ├── logo.png
│   │   ├── banner.jpg

```



However, if you plan to run it on different architectures (e.g., x86 vs. ARM), you should consider the following points:

Works on These Systems:
✔ Linux (Ubuntu, Debian, Fedora, etc.) – Runs natively.
✔ MacOS – Works without modification.
✔ Windows (WSL) – Works via Windows Subsystem for Linux.
✔ Embedded Devices (Raspberry Pi, Jetson Nano, etc.) – Works with cross-compilation.


**Things to Consider When Running on Other Systems**
1. Port Binding Permissions (Linux & MacOS)

a. Ports below 1024 require root access. If you're using port 80, run with:
     sudo ./server

b. Or, use a non-privileged port (e.g., 3000, 8080).

2. Architecture Differences (x86 vs. ARM)

a. If you're running on Raspberry Pi (ARM CPU) but compiling on PC (x86 CPU), you need cross-compilation:
      
      g++ -o server server.cpp TcpServer.cpp -march=armv7-a

3. Windows Compatibility

a. Windows does not support <unistd.h>. Use WSL or MinGW to compile.

4. Firewall Rules (Linux & Windows)

a. Some systems block incoming TCP connections by default.

b. Open the port manually using:
    sudo ufw allow 8080/tcp


To test the server in your Linux/ Mac OS server:
Step 1. Clone the repo

Step 2. Make sure the g++ compiler is already installed, if not, then install the g++.

Step 3. Open the terminal, run below cmd:
        
        g++ -o server server.cpp TcpServer.cpp
      
or, can run:

        make -f MakeFile.txt

Step 4. Once everything is compiled, a bytecode file will be generated in the same folder as "server"
        
        run below cmd:
          
          ./server

Step 5. Open any browser and give the address:
        
        http://localhost:8080

Note: If you are also starting the server in Raspberry Pi, then give ip of your Raspberry Pi instead of localhost.
