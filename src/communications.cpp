#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#define PORT 54000
#define BUFFER_SIZE 1024

//extern std::vector<std::string> chatHistory;
// extern std::mutex chatMutex;

// extern std::string lastReceivedMessage;
// extern std::mutex lastMessageMutex;


void startServerSocket(SOCKET& serverSocket, SOCKET& connectionSocket) {
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "[Serwer] Nasłuchiwanie na porcie " << PORT << "...\n";

    connectionSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);
    std::cout << "[Serwer] Połączono z klientem.\n";

    // std::lock_guard<std::mutex> lock(chatMutex);
    // chatHistory.push_back("[System] Połączono z klientem.");
}

void startClientSocket(SOCKET& clientSocket, const std::string& ip) {
    sockaddr_in serverAddr;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    std::cout << "[Klient] Łączenie z serwerem...\n";

    while (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "[Klient] Połączono z serwerem.\n";

    // std::lock_guard<std::mutex> lock(chatMutex);
    // chatHistory.push_back("[System] Połączono z serwerem.");
}

std::string receiveMessages(SOCKET socket, const std::string& prefix) {
    char buffer[BUFFER_SIZE];
    std::string msg = "";

    int bytesReceived = recv(socket, buffer, BUFFER_SIZE - 1, 0);
    if (bytesReceived <= 0) {
        return "[System] Rozłączono.";
    }
    buffer[bytesReceived] = '\0';

    msg = prefix + ": " + buffer;

    std::cout << msg << std::endl;

    // Dodaj wiadomość do historii czatu, ale już po otrzymaniu jej
    // std::lock_guard<std::mutex> lock(chatMutex);
    // chatHistory.push_back(msg);
    // lastReceivedMessage = buffer;

    return buffer;  // Zwracamy odebraną wiadomość
}


void sendMessages(SOCKET socket, const std::string& msg, const std::string& prefix) {
    if (msg.empty()) return;
    send(socket, msg.c_str(), msg.length(), 0);

    // std::lock_guard<std::mutex> lock(chatMutex);
    // chatHistory.push_back(prefix + ": " + msg);
}


// int test() {
//     WSADATA wsaData;
//     WSAStartup(MAKEWORD(2, 2), &wsaData);  // Uruchomienie biblioteki winsoc

//     std::cout << "Tryb (1 - Serwer, 2 - Klient): "; //wybor trybu serera (klient/serwer)
//     int mode;
//     std::cin >> mode;
//     std::cin.ignore();

//     SOCKET socket1, socket2;

//     if (mode == 1) {
//         startServerSocket(socket1, socket2); // Jeśli serwer to otwieramy socket
//     } else {
//         std::string ip;
//         std::cout << "Podaj IP serwera (np. 127.0.0.1): ";
//         std::cin >> ip;
//         std::cin.ignore();
//         startClientSocket(socket2, ip); // Jesli klinet to lączymy z socetem
//     }

//     // Wątek odbierający wiadomości
//     std::thread receiver(receiveMessages, (mode == 1 ? socket2 : socket2), (mode == 1 ? "Klient" : "Serwer")); 

//     // Wątek wysyłający wiadomości
//     sendMessages(socket2, ""); 

//     closesocket(socket2);
//     if (mode == 1) closesocket(socket1);
//     WSACleanup();

//     return 0;
// }
