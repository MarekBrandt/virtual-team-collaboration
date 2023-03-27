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

enum FrameType {
    NORMAL,
    UNREGISTER,
    CONNECT,
    EMPTY
};

struct Frame {
    int type;
    int iID;
    ObjectState state;
};

struct Client {
    unsigned long address;
    int id = -1;
    Frame lastFrame = Frame{FrameType::EMPTY};
};

std::vector<Client*> clients{};

Frame currentFrame;
Client* currentClient;

Client* findClient(unsigned long* address, int id) {
    for (Client* client : clients) {
        if (client->id == id) {
            return client;
        }
    }
    Client* newClient = new Client{ *address, id };
    clients.push_back(newClient);
    return newClient;
}

boolean resolveConnection() {
    Frame lastFrame = currentClient->lastFrame;
    if (lastFrame.type == FrameType::EMPTY) return false;
    if (lastFrame.iID != currentFrame.iID) return false;
    return true;
}

//void removeFromClients(unsigned long* address) {
//    int index = 0;
//    for (auto client : clients) {
//        if (client->address == *address) {
//            clients.erase(clients.begin() + index);
//            return;
//        }
//    }
//}

int main()
{
    bool isRunning = true;
    unicast_net uni_reciv = unicast_net::unicast_net(1001);
    unicast_net uni_send = unicast_net::unicast_net(1002);
    while (isRunning) {
        printf("\n\n");

        for (auto client : clients) {
            printf("client: %ld\n", client->id);
        }
        printf("\n\n");
        unsigned long ip_sender = 0;
        int size = uni_reciv.reciv((char*)&currentFrame, &ip_sender, sizeof(Frame));
        printf("otrzymalem wiadomosc od: %ld\n", ip_sender);
        printf("id: %d\n", currentFrame.iID);
        currentClient = findClient(&ip_sender, currentFrame.iID);
        if (currentFrame.type == FrameType::UNREGISTER) {
            currentClient->lastFrame = currentFrame;
            //removeFromClients(&ip_sender);
        }
        else if(currentFrame.type == FrameType::CONNECT){
            if (resolveConnection()) { 
                currentFrame.state = currentClient->lastFrame.state;
                uni_send.send((char*)&currentFrame, currentClient->address, sizeof(Frame)); 
            }
            currentFrame.type = FrameType::NORMAL;
        }
        else {
            currentFrame.type = FrameType::NORMAL;
        }
        for (auto client : clients) {
            printf("client: %ld\n", client->address);
            uni_send.send((char*)&currentFrame, client->address, sizeof(Frame));
        }
       
    }
}