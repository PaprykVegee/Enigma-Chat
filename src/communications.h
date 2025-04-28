// communications.h
#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

std::string receiveMessages(SOCKET socket, const std::string& prefix);
void sendMessages(SOCKET socket, const std::string& msg, const std::string& prefix);
void startClientSocket(SOCKET& clientSocket, const std::string& ip);
void startServerSocket(SOCKET& serverSocket, SOCKET& connectionSocket);

#endif // COMMUNICATIONS_H
