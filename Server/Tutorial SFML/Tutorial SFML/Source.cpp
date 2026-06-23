#include <SFML/Network.hpp>
#include <iostream>
#include "NetworkManager.h"

int main()
{
    if(!NT->Init()) return -1;
    DB->Connect();
    while (NT->Update());
    DB->Disconnect();
}