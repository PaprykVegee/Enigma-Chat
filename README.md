# Project: Enigma Cipher Chat

## Description
This project is a chat application that uses the Enigma cipher to encrypt and decrypt messages. Users can communicate in client or server modes. All messages are encrypted before sending and decrypted upon receiving. The user interface is based on **ImGui**, and communication occurs via **WinSock2**.

## Requirements
- **ImGui** – for creating the graphical user interface
- **OpenGL** – for rendering the interface
- **GLFW** – for window and input handling
- **WinSock2** – for handling network communication
- **Enigma** – Enigma cipher implementation

## Key Features
1. **Graphical Interface**:
   - Mode selection: **Client** or **Server**
   - Message input
   - Display chat history
   - Enigma cipher settings (rotor positions)

2. **Enigma Cipher**:
   - Encrypting/decrypting messages
   - Adjusting Enigma settings (rotor positions)

3. **Network Communication**:
   - **Server**: Listens on a port and receives messages from clients
   - **Client**: Connects to the server and sends messages

4. **Sockets (WinSock2)**:
   - Used for communication between the client and server

## Installation
1. Install required libraries: ImGui, OpenGL, GLFW, WinSock2.
2. Configure the project in your development environment.
3. Run the application and select the mode (client/server).

## Running
- **Server**: Run the application in server mode, listening on a port.
- **Client**: Run the application in client mode, enter the server's IP address, and connect.

## Usage
1. **Client and Server**: Select the mode and click "Connect".
2. Type a message and click "Send" to send it to the other side.
3. The chat history will be displayed in the chat window, with the prefix "Server:" or "Client:".

## License
This project is available under the MIT license.
