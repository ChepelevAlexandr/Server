#pragma once

// Функции для обработки игры
void handleMove(int board[3][3], int row, int col, int player);
int checkWinner(int board[3][3]);
void handleGame(int player1Socket, int player2Socket);
