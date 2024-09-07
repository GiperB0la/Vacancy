#pragma once
#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <mutex>
#include <cstring>


class Client
{
public:
    Client(std::string name, int port, int period);
    ~Client();

public:
    void connection();

private:
    void init();
    void sending();
    std::string getCurrentTimeFormatted();

private:
    sockaddr_in info;
    int clientSocket;

private:
    int port;
    int period;
    std::mutex mx;
    std::string name;
    std::string ipAddress;

private:
    bool run;
};