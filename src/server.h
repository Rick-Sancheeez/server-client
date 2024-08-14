// server.h
#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

class Server {
public:
    Server(const std::string& ip, int port);
    ~Server();
    void Start();

private:
    SOCKET ServSock;
    std::string ipAddress;
    int portNumber;
    sockaddr_in servInfo;

    void InitializeWinSock();
    void CreateSocket();
    void BindSocket();
    void ListenForClients();
    void HandleClient(SOCKET clientSocket);
};

#endif
