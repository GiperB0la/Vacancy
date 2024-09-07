#include "../include/Server.h"


int main()
{
    Server server(55555);
    server.start();

    return 0;
}