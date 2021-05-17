#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <endpointvolume.h>
#include <ctime>
#include <kiss_fft.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "192.168.1.18"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 4210	//The port on which to listen for incoming data

const int nfft = 1024;
const float freq = 31250;
const float peri = 1 / freq;
const float c_peri = 1000000 * peri;
void createMessage();

int main(void)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char message[BUFLEN] = "send";
	WSADATA wsa;

	kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
	float in[nfft];
	kiss_fft_cpx out[nfft];

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
	clock_t c_count_old = clock();
	clock_t c_count_new = clock();

	while (true)
	{
		if (c_count_new - c_count_old >= c_peri)
		{
			c_count_old = c_count_new;
			for (int i = nfft - 1; i > 1; i--)
			{
				in[i] = in[i - 1];
			}
			float newVol;
			//IAudioEndpointVolume::GetMasterVolumeLevelScalar(&newVol);
			kiss_fftr(cfg, in, out);
		}
		c_count_new = clock();
	}
	//send the message
	if (sendto(s, message, strlen(message), 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	closesocket(s);
	WSACleanup();

	kiss_fftr_free(cfg);
	return 0;
}

void createMessage() {

}
