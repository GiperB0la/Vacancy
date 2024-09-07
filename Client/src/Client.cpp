#include "../include/Client.h"


Client::Client(std::string name, int port, int period)
    : info{ 0 }, clientSocket(-1), ipAddress("127.0.0.1"),
      name(name), port(port), period(period), run(false)
{}

Client::~Client()
{
    if (clientSocket != -1)
        close(clientSocket);
}

void Client::init()
{
    std::this_thread::sleep_for(std::chrono::seconds(period));

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << std::endl << "[-] Error creating socket" << std::endl;
        return;
    }

    info.sin_family = AF_INET;
    info.sin_port = htons(port);

    if (inet_pton(AF_INET, ipAddress.c_str(), &info.sin_addr) <= 0) {
        std::cerr << "[-] Invalid address" << std::endl;
        return;
    }

    if (connect(clientSocket, (sockaddr*)&info, sizeof(info)) < 0) {
        std::cerr << "[-] Connection failed" << std::endl;
        close(clientSocket);
        return;
    }

    run = true;
    std::cout << "[+] Connected to server." << std::endl;
}

void Client::connection()
{
    init();

    while (true) {
        if (run)
            sending();
        else
            init();
    }
}

void Client::sending()
{
    std::lock_guard<std::mutex> lock(mx);

    std::string message = getCurrentTimeFormatted() + " " + name;
    std::cout << message << std::endl;
    send(clientSocket, message.c_str(), message.size(), 0);

    std::this_thread::sleep_for(std::chrono::seconds(period));
}

std::string Client::getCurrentTimeFormatted()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* tm = std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << "." << std::setw(3) << std::setfill('0') << milliseconds.count();

    return oss.str();
}