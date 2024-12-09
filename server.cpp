#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error al iniciar Winsock" << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error al crear el socket" << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error al hacer el bind" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        cerr << "Error al escuchar conexiones" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Esperando conexiones..." << endl;

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Error al aceptar la conexión" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Conexión establecida con el cliente." << endl;

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived == SOCKET_ERROR) {
        cerr << "Error al recibir el mensaje" << endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    buffer[bytesReceived] = '\0';
    cout << "Mensaje recibido del cliente: " << buffer << endl;

    const char* response = "Hola des del servidor!";
    int bytesSent = send(clientSocket, response, strlen(response), 0);
    if (bytesSent == SOCKET_ERROR) {
        cerr << "Error al enviar la respuesta" << endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Respuesta enviada al cliente." << endl;

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
