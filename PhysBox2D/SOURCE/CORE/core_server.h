#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include "fundamental_singleton.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <vector>
#include <functional>
#include <iostream>    
#include <sstream>  
#include <string> 
#include <iomanip>

#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT "1986"

class CORE_SERVER : public FUNDAMENTAL_SINGLETON< CORE_SERVER > {
public:
	std::thread													m_thread;
	std::map<std::string, std::function<std::string()>>			m_bindings;

	union CONVERTER {
		float		float_value;
		int			integer_value;
		unsigned	bits;
	};

	~CORE_SERVER() {
		m_thread.detach();
	}
	void add_binding(const std::string& command, std::function<std::string()> func) {
		m_bindings[command] = func;
	}

	void run()
	{
		m_thread = std::thread(&CORE_SERVER::start, this);
	}

	int start()
	{
		WSADATA wsaData;
		int iResult;

		SOCKET ListenSocket = INVALID_SOCKET;
		SOCKET ClientSocket = INVALID_SOCKET;

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		int iSendResult;
		char recvbuf[DEFAULT_BUFLEN];
		int recvbuflen = DEFAULT_BUFLEN;

		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (ListenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		freeaddrinfo(result);

		iResult = listen(ListenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}

		closesocket(ListenSocket);

		do {
			std::string reply;
			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

			if (iResult > 0) {
				//printf("Bytes received: %d\n", iResult);

				std::string command;

				for (size_t i = 0; i < iResult; i++) {
					command.push_back(recvbuf[i]);
				}
				reply = m_bindings[command]();

				iSendResult = send(ClientSocket, std::to_string(reply.size()).c_str(), std::to_string(reply.size()).size(), 0);

				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				//printf("Bytes sent: %d\n", iSendResult);
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

			if (reply.empty()) {
				continue;
			}

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				//printf("Bytes received: %d\n", iResult);

				iSendResult = send(ClientSocket, reply.c_str(), int(reply.size()), 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}

				//printf("Bytes sent: %d\n", iSendResult);
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else {
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}

		} while (iResult > 0);

		iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

		closesocket(ClientSocket);
		WSACleanup();

		return 0;
	}

	static void pack_float(const float value, std::string& reply) {

		CONVERTER			converter;
		std::ostringstream	replyStream;

		std::string new_reply;

		converter.float_value = value;


		replyStream << std::setw(8) << std::setfill('0') << std::hex << converter.bits;

		new_reply = replyStream.str();

		reply += new_reply;
	}

	static void pack_int(const int value, std::string& reply) {

		CONVERTER			converter;
		std::ostringstream	replyStream;

		std::string new_reply;

		converter.integer_value = value;

		new_reply = replyStream.str();

		replyStream << std::setw(8) << std::setfill('0') << std::hex << converter.bits;

		new_reply = replyStream.str();

		reply += new_reply;
	}

	static void pack_image(unsigned char* pixels, const int width, const int height, std::string& reply) {
		CONVERTER			converter;
		std::ostringstream	replyStream;

		for (size_t i = 0; i < width * height; i++) {
			std::ostringstream	internalStream;
			
			converter.integer_value = pixels[i];
			internalStream << std::setw(2) << std::setfill('0') << std::hex << converter.bits;

			replyStream << internalStream.str();
		}
		reply = replyStream.str();
		std::cout << reply.size() << std::endl;
	}
};
