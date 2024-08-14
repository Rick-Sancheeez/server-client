// server.cpp
#include "server.h"
#include "utils.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>

Server::Server(const std::string& ip, int port) : ipAddress(ip), portNumber(port) {
    InitializeWinSock();
    CreateSocket();
    BindSocket();
}

Server::~Server() {
    closesocket(ServSock);
    WSACleanup();
}

void Server::InitializeWinSock() {
    WSADATA wsData;
    int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
    if (erStat != 0) {
        throw std::runtime_error("WinSock initialization failed");
    }
}

void Server::CreateSocket() {
    ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ServSock == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed");
    }
}

void Server::BindSocket() {
    in_addr ip_to_num;
    int erStat = inet_pton(AF_INET, ipAddress.c_str(), &ip_to_num);
    if (erStat <= 0) {
        throw std::runtime_error("Invalid IP address format");
    }

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(portNumber);

    erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));
    if (erStat != 0) {
        throw std::runtime_error("Socket binding failed");
    }
}

void Server::ListenForClients() {
    int erStat = listen(ServSock, SOMAXCONN);
    if (erStat != 0) {
        throw std::runtime_error("Listening failed");
    }

    while (true) {
        sockaddr_in clientInfo;
        int clientInfoSize = sizeof(clientInfo);
        SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfoSize);

        if (ClientConn == INVALID_SOCKET) {
            std::cerr << "Client connection failed" << std::endl;
            continue;
        }

        std::thread(&Server::HandleClient, this, ClientConn).detach();
    }
}

void Server::HandleClient(SOCKET clientSocket) {
    std::vector<char> servBuff(1024), clientBuff(1024);
    short packetSize;

    while (true) {
        packetSize = recv(clientSocket, servBuff.data(), servBuff.size(), 0);
        if (packetSize <= 0) break;
        servBuff[packetSize] = '\0';

        std::string clientMessage(servBuff.data());
        std::string resultMessage;
        try {
            int result = Math(clientMessage);
            resultMessage = "Result: " + std::to_string(result);
        } catch (const std::invalid_argument& e) {
            resultMessage = e.what();
        }

        packetSize = send(clientSocket, resultMessage.c_str(), resultMessage.size(), 0);
        if (packetSize == SOCKET_ERROR) break;
    }

    shutdown(clientSocket, SD_BOTH);
    closesocket(clientSocket);
}

void Server::Start() {
    ListenForClients();
}
