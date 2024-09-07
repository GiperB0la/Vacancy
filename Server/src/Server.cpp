#include "../include/Server.h"


Server::Server(int port) : port(port), ipAddress("127.0.0.1"),
    serverSocket(-1), info{ 0 }
{}

Server::~Server()
{
    if (serverSocket != -1)
        close(serverSocket);
}

void Server::init()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << std::endl << "[-] Error creating socket" << std::endl;
        return;
    }

    info.sin_family = AF_INET;
    info.sin_port = htons(port);
    info.sin_addr.s_addr = inet_addr(ipAddress.c_str());

    if (bind(serverSocket, (sockaddr*)&info, sizeof(info)) < 0) {
        std::cerr << std::endl << "[-] Error binding socket" << std::endl;
        return;
    }

    std::cout << "[+] The server is running." << std::endl;
}

void Server::start()
{
    init();

    std::thread connectingThread(&Server::listening, this);
    connectingThread.detach();

    while (true) {

    }
}

void Server::listening()
{
    if (listen(serverSocket, SOMAXCONN) < 0) {
        std::cerr << std::endl << "[-] Error listening" << std::endl;
        close(serverSocket);
        return;
    }

    while (true) {
        sockaddr_in clientInfo;
        socklen_t clientInfoLength = sizeof(clientInfo);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientInfo, &clientInfoLength);

        if (clientSocket < 0) {
            std::cerr << "[-] Accept failed" << std::endl;
            continue;
        }
        std::cout << "[+] Client connected from: " << inet_ntoa(clientInfo.sin_addr) << ":" << ntohs(clientInfo.sin_port) << std::endl;
        std::thread(&Server::handleClient, this, clientSocket).detach();
    }
}

void Server::handleClient(int clientSocket)
{
    char buffer[512] = {0};
    while (true) {
        int recvLength = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (recvLength <= 0) {
            std::cout << "Client disconnected." << std::endl;
            close(clientSocket);
            break;
        }
        
        std::lock_guard<std::mutex> lock(mx);
        std::ofstream logFile("../../log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << buffer << std::endl;
            logFile.close();
        }
        std::cout << buffer << std::endl;
        memset(buffer, 0, sizeof(buffer));
    }
}