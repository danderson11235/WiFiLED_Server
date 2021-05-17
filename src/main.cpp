#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <endpointvolume.h>
#include <time.h>
#include "../include/kiss_fft130/kiss_fft.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "192.168.1.18"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 4210	//The port on which to listen for incoming data

void createMessage();

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN] = "send";
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);

	//start communication

	while (true)
	{
		
	}
	//send the message
	if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}

void createMessage() {

}
