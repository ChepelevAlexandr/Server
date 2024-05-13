#include <iostream>
#include <winsock2.h>
#include "game.h"

using namespace std;

int main() {
    // ������������� Winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0) {
        cerr << "Can't initialize Winsock! Quitting" << endl;
        return 1;
    }

    // �������� ������ ��� ������ 1
    SOCKET player1Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (player1Socket == INVALID_SOCKET) {
        cerr << "Can't create player 1 socket! Quitting" << endl;
        WSACleanup();
        return 1;
    }

    // �������� ������ ��� ������ 1
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;
    bind(player1Socket, (sockaddr*)&hint, sizeof(hint));

    // �������� ����������� ������ 1
    listen(player1Socket, SOMAXCONN);
    sockaddr_in client;
    int clientSize = sizeof(client);
    SOCKET clientSocket = accept(player1Socket, (sockaddr*)&client, &clientSize);

    // �������� ������ ��� ������ 2
    SOCKET player2Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (player2Socket == INVALID_SOCKET) {
        cerr << "Can't create player 2 socket! Quitting" << endl;
        closesocket(player1Socket);
        WSACleanup();
        return 1;
    }

    // �������� ������ ��� ������ 2
    bind(player2Socket, (sockaddr*)&hint, sizeof(hint));

    // �������� ����������� ������ 2
    listen(player2Socket, SOMAXCONN);
    SOCKET client2Socket = accept(player2Socket, (sockaddr*)&client, &clientSize);

    // ��������� ����
    handleGame(clientSocket, client2Socket);

    // �������� ������� � ������� Winsock
    closesocket(clientSocket);
    closesocket(client2Socket);
    WSACleanup();

    return 0;
}
