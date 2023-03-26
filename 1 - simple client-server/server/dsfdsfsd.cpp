#include <iostream>
#include <windows.h>
#include <math.h>
#include <time.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "objects.h"
#include <vector>
#include <algorithm>
#include "net.h"

struct Client {
    unsigned long address;
};

enum FrameType {
    NORMAL,
    UNREGISTER
};

struct Frame {
    int type;
    int iID;
    ObjectState state;
};

std::vector<Client*> clients{};

void addToClients(unsigned long* address) {
    for (auto client : clients) {
        if (client->address == *address) {
            return;
        }
    }
    clients.push_back(new Client{ *address });
}

void removeFromClients(unsigned long* address) {
    int index = 0;
    for (auto client : clients) {
        if (client->address == *address) {
            clients.erase(clients.begin() + index);
            return;
        }
    }
}

int main()
{
    bool isRunning = true;
    Frame frame;
    unicast_net uni_reciv = unicast_net::unicast_net(1001);
    unicast_net uni_send = unicast_net::unicast_net(1002);
    while (isRunning) {
        printf("\n\n");

        for (auto client : clients) {
            printf("client: %ld\n", client->address);
        }
        printf("\n\n");
        unsigned long ip_sender = 0;
        int size = uni_reciv.reciv((char*)&frame, &ip_sender, sizeof(Frame));
        printf("otrzymalem wiadomosc od: %ld\n", ip_sender);
        printf("id: %d\n", frame.iID);
        if (frame.type == FrameType::UNREGISTER) {
            removeFromClients(&ip_sender);
        }
        else {
            addToClients(&ip_sender);
        }
        for (auto client : clients) {
            printf("client: %ld\n", client->address);
            uni_send.send((char*)&frame, client->address, sizeof(Frame));
        }
       
    }
}