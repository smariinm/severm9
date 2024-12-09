#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al iniciar Winsock" << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error al crear el socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error al conectar con el servidor" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Conexión establecida con el servidor." << endl;

    const char* message = "Hola servidor!";
    int bytesSent = send(clientSocket, message, strlen(message), 0);
    if (bytesSent == SOCKET_ERROR) {
        cerr << "Error al enviar el mensaje" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Enviando mensaje: " << message << endl;

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        cerr << "Error al recibir la respuesta" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    buffer[bytesReceived] = '\0';
    cout << "Respuesta del servidor: " << buffer << endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
