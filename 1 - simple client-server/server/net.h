//
#ifndef _NETWORK__H
#define _NETWORK__H

// VERSION FOR WINDOWS
#ifdef WIN32
//#include <ws2tcpip.h>       // uncomment it in console application!
#else // VERSION FOR LINUX
#include <arpa/inet.h>  
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

class unicast_net
{
public:
	int sock;                         // Socket 
	struct sockaddr_in udpServAddr; // Local address 
	struct sockaddr_in udpClntAddr; // Client address 

	unsigned long udpIP;             // IP  address 
	unsigned short udpPort;     // Server port 

	short sSize;

#ifdef WIN32
	WSADATA localWSA; //***************** for Windows
#endif

	unicast_net(unsigned short); // port number
	~unicast_net();

	int send(char*, unsigned long, unsigned short); // pointer to data, IP Adres (unsigned long), size of data
	int reciv(char*, unsigned long*, unsigned short); // pointer to buffer,Sender IP Adres (unsigned long), size recive buffer

	int send(char*, char*, unsigned short); // pointer to data, IP Adres (string), size of data

};


#endif
