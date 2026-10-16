#include <iostream>
#include <WS2tcpip.h>
#include "udp-broadcaster-plugin.hpp";

// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")

// Saves us from typing std::cout << etc. etc. etc.
using namespace std;
using namespace rFactor2UdpBroadcaster;

int PORT = 60000;

// Main entry point into the server
void main()
{
	////////////////////////////////////////////////////////////
	// INITIALIZE WINSOCK
	////////////////////////////////////////////////////////////

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I create a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		cout << "Can't start Winsock! " << wsOk;
		return;
	}

	////////////////////////////////////////////////////////////
	// SOCKET CREATION AND BINDING
	////////////////////////////////////////////////////////////

	// Create a socket, notice that it is a user datagram socket (UDP)
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(PORT); // Convert from little to big endian

	// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket! " << WSAGetLastError() << endl;
		return;
	}

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information

	auto szHeader = sizeof(PacketHeader);
	size_t bufferSz = 4096; // Maximum size that the buffer will ever be
	char* buffer = (char*)malloc(bufferSz); //to receive data

	// Enter a loop
	for(;;)
	{
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buffer, bufferSz); // Clear the receive buffer (memset)

		//Receive a network packet and copy in to buffer
		auto bytesRead = recvfrom(in, buffer, bufferSz, 0, (sockaddr*)&client, &clientLength);
		if (bytesRead < 0)
		{
			auto err = WSAGetLastError();
			cout << "Error receiving from client " << err << endl;
			continue;
		}

		DataPacket data;
		data.ParseFromArray(buffer, bytesRead);

		// Now retrieve the data based on the header
		if (data.packetheader().packettype() == PacketType::TELEMETRY)
		{
			cout << " : " << "Time: " << data.telemetry().melapsedtime()
				//<< " , RPM: " << data.telemetry().mEngineRPM()
				<< endl;
		}
		else if (data.packetheader().packettype() == PacketType::SCORING)
		{
			cout << "Lap Dist: " << data.scoring().mlapdist() << endl;
		}
		else if (data.packetheader().packettype() == PacketType::GRAPHICS)
		{
			cout << " : " << "Cam Pos: " <<
				data.graphics().mcampos().x() << "," <<
				data.graphics().mcampos().y() << "," <<
				data.graphics().mcampos().z() << endl;
		}

		// Display message and client info
		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		// Convert from byte array to chars
		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);
	}

	// Close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
}
