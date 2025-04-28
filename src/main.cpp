#include <map>
#include <vector>
#include <string>
#include <mutex>

#include <Enigma_Module.h>
#include <GUI.h>
#include <communications.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <random>
#include <thread>
#include <winsock2.h>

std::vector<std::string> chatHistory;
std::vector<std::string> LocalChatHistory; //

std::mutex chatMutex;
std::string lastReceivedMessage;

bool isConnecting = false;
bool isConnected = false;
bool receiverStarted = false;

SOCKET socket1 = INVALID_SOCKET, socket2 = INVALID_SOCKET;

void render_UI() {
    static int rotor_pos1 = 0;
    static int rotor_pos2 = 0;
    static int rotor_pos3 = 0;
    static char inputText[256] = "";
    static char ip_text[256] = "127.0.0.1";
    static const char* SerwerClient[] = { "Klient", "Serwer" };
    static int selectedIndex = 0;

    static int seed_root1 = 42;
    static int seed_root2 = 1337;
    static int seed_root3 = 2024; 

    static std::map<char, char> recorde_table;
    static Enigma Enigma_encoder(seed_root1, seed_root2, seed_root3, rotor_pos1, rotor_pos2, rotor_pos3, 1, 1, 1, recorde_table);
    static Enigma Enigma_decoder(seed_root1, seed_root2, seed_root3, rotor_pos1, rotor_pos2, rotor_pos3, 1, 1, 1, recorde_table);

    int offset = 1100 / 2;

    ImGui::Begin("MainUI");

    // Rotory
    rotor(seed_root1, rotor_pos1, 50 + offset - 200, 50);
    rotor(seed_root2, rotor_pos2, 50 + offset, 50);
    rotor(seed_root3, rotor_pos3, 50 + offset + 200, 50);

    recorde_table = Recorde_Table(50, 550);

    ImGui::SetNextItemWidth(100.0f);
    ImGui::SetCursorPos(ImVec2(2 * offset + 50, 50));
    ImGui::Combo("Tryb", &selectedIndex, SerwerClient, IM_ARRAYSIZE(SerwerClient));

    if (selectedIndex == 0) {
        ImGui::SetNextItemWidth(100.0f);
        ImGui::SetCursorPos(ImVec2(2 * offset + 50, 100));
        ImGui::InputText("Adres IP", ip_text, IM_ARRAYSIZE(ip_text));
    }

    ImGui::SetCursorPos(ImVec2(2 * offset + 200, 100));
    bool connectButton = ImGui::Button("Połącz");

    // Okno czatu
    ImVec2 child_pos = ImVec2(100, 200);
    ImVec2 child_size = ImVec2(950, 300);
    ImGui::SetCursorPos(child_pos);
    ImGui::BeginChild("BlueFrame", child_size, true);

    {
        std::lock_guard<std::mutex> lock(chatMutex);
        for (const std::string& msg : LocalChatHistory) {
            ImGui::TextWrapped("%s", msg.c_str());
        }
    }

    ImGui::EndChild();

    // Wysyłanie wiadomości
    ImGui::SetCursorPos(ImVec2(50, 510));
    ImGui::SetNextItemWidth(1050);
    ImGui::InputText("Wiadomość", inputText, IM_ARRAYSIZE(inputText));

    ImGui::SetCursorPos(ImVec2(2 * offset + 150, 510));
    bool sendButton = ImGui::Button("Wyślij");

    // Zastosowanie nowej Enigmy
    ImGui::SetCursorPos(ImVec2(2 * offset + 150, 550));
    bool applyEngimaSetting = ImGui::Button("Zastosuj");

    if (applyEngimaSetting) {
        Enigma_encoder = Enigma(seed_root1, seed_root2, seed_root3, rotor_pos1, rotor_pos2, rotor_pos3, 1, 1, 1, recorde_table);
        Enigma_decoder = Enigma(seed_root1, seed_root2, seed_root3, rotor_pos1, rotor_pos2, rotor_pos3, 1, 1, 1, recorde_table);
        std::cout << "Stworzono Enigmę" << std::endl;
    }

    // Łączenie
    if (connectButton && !isConnected && !isConnecting) {
        isConnecting = true;
        std::thread([=]() mutable {
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
                std::cerr << "WSAStartup failed" << std::endl;
                isConnecting = false;
                return;
            }

            if (selectedIndex == 1) {
                startServerSocket(socket1, socket2);
            } else {
                startClientSocket(socket2, ip_text);
            }

            isConnected = true;
            isConnecting = false;
        }).detach();
    }

    // Odbieranie wiadomości
    if (isConnected && !receiverStarted) {
        receiverStarted = true;
        std::thread([]() {
            while (isConnected) {
                std::string received = receiveMessages(socket2, "Receiver");

                if (!received.empty()) {
                    std::string decodedMessage;
                    for (char c : received) {
                        c = std::toupper(c);
                        if (c == ' ') {
                            decodedMessage += c;
                        } else {
                            decodedMessage += Enigma_decoder.encrypte(c);
                        }
                    }
                    std::lock_guard<std::mutex> lock(chatMutex);
                    LocalChatHistory.push_back(decodedMessage);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }).detach();
    }

    // Wysyłanie wiadomości
    if (sendButton && isConnected && strlen(inputText) > 0) {
        std::string encodedInputText;
        for (char c : inputText) {
            c = std::toupper(c);
            if (c == ' ') {
                encodedInputText += c;
            } else {
                encodedInputText += Enigma_encoder.encrypte(c);
            }
        }
        sendMessages(socket2, encodedInputText, selectedIndex == 1 ? "Serwer" : "Klient");

        {
            std::lock_guard<std::mutex> lock(chatMutex);
            LocalChatHistory.push_back(std::string(inputText)); // Dodaj lokalnie oryginalny tekst
        }

        inputText[0] = '\0'; // Czyść pole tekstowe
    }

    ImGui::End();
}

    
int main()
{
    // === Inicjalizacja GLFW ===
    if (!glfwInit())
        return -1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui - Historia w jednym oknie", NULL, NULL);
    if (window == NULL)
        return -1;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // VSync

    // === Inicjalizacja ImGui ===
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // === Zainicjalizuj dane UI ===
    DefineUI();

    // === Główna pętla ===
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Nowa klatka
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        render_UI();
        
        // Renderuj GUI
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffer
        glfwSwapBuffers(window);
    }

    // === Sprzątanie ===
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

