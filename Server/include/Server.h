#pragma once
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


class Server
{
public:
    Server(int port);
    ~Server();

public:
    void start();

private:
    void init();
    void listening();
    void handleClient(int clientSocket);

private:
    sockaddr_in info;
    int serverSocket;

private:
    int port;
    std::mutex mx;
    std::string ipAddress;
};