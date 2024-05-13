#include "game.h"
#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

// Функция для обработки хода игрока
void handleMove(int board[3][3], int row, int col, int player) {
    board[row][col] = player;
}

// Функция для проверки победителя
int checkWinner(int board[3][3]) {
    // Проверяем строки
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    // Проверяем столбцы
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] != 0 && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            return board[0][j];
        }
    }

    // Проверяем главную диагональ
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }

    // Проверяем побочную диагональ
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }

    // Проверяем наличие пустых клеток (ничья)
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == 0) {
                return 0; // Продолжаем игру
            }
        }
    }

    // Ничья
    return -1;
}

// Функция для обработки игры
void handleGame(int player1Socket, int player2Socket) {
    int board[3][3] = { 0 };
    int currentPlayer = 1;

    // Отправляем сообщение об успешном подключении обоим игрокам
    string successMsg = "Connected! You are player " + to_string(currentPlayer);
    send(player1Socket, successMsg.c_str(), successMsg.size() + 1, 0);
    send(player2Socket, successMsg.c_str(), successMsg.size() + 1, 0);

    while (true) {
        // Отправляем текущее состояние игры игрокам
        send(player1Socket, (char*)board, sizeof(board), 0);
        send(player2Socket, (char*)board, sizeof(board), 0);

        // Проверяем состояние игры (победа, ничья или продолжение игры)
        int winner = checkWinner(board);
        if (winner != 0) {
            // Отправляем результат игрокам
            send(player1Socket, (char*)&winner, sizeof(winner), 0);
            send(player2Socket, (char*)&winner, sizeof(winner), 0);
            break;
        }

        // Ждем ход от текущего игрока
        int currentPlayerSocket = (currentPlayer == 1) ? player1Socket : player2Socket;
        int row, col;
        recv(currentPlayerSocket, (char*)&row, sizeof(row), 0);
        recv(currentPlayerSocket, (char*)&col, sizeof(col), 0);

        // Обрабатываем ход
        handleMove(board, row, col, currentPlayer);

        // Меняем игрока
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
    }

    // Закрываем соединение с игроками после окончания игры
    closesocket(player1Socket);
    closesocket(player2Socket);
}
