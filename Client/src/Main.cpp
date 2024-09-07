#include "../include/Client.h"


int main()
{
    std::vector<std::thread> threads;

    Client client1("Client1", 55555, 1);
    Client client2("Client2", 55555, 2);
    Client client3("Client3", 55555, 3);

    threads.emplace_back(&Client::connection, &client1);
    threads.emplace_back(&Client::connection, &client2);
    threads.emplace_back(&Client::connection, &client3);

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}