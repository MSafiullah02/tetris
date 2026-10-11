#include "screen.h"
#include <iostream>
#include <random>
std::mutex screenMutex;
screen Screen;
std::atomic<bool> game = true;
std::mutex movementMutex;

screen::screen() {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 12; j++) {
            s[i][j] = ' ';
        }
    }
}

void screen::addBlock(char n) {
    std::lock_guard<std::mutex> lock(screenMutex);

    block b = block::getBlock(n);

    x = 0;
    y = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b.b[i][j] != ' ' && s[i][j] != ' ') {
                game = false;
                return;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b.b[i][j] != ' ') {
                s[i][j] = b.b[i][j];
            }
        }
    }

    id = b.id;
}

bool screen::canMove(char m) {
    // 1 = left
    // 2 = down
    // 3 = right

    int dx = 0;
    int dy = 0;

    if (m == 1)
        dx = -1;
    else if (m == 2)
        dy = 1;
    else if (m == 3)
        dx = 1;

    for (int i = 0; i < 4 && x + i < 12; i++) {
        for (int j = 0; j < 4 && y + j < 12; j++) {

            if (s[y + j][x + i] != id)
                continue;

            int newX = x + i + dx;
            int newY = y + j + dy;

            // Would the cell leave the screen?
            if (newX < 0 || newX >= 12 || newY < 0 || newY >= 12) {
                return false;
            }

            // Would it collide with another block?
            if (s[newY][newX] != ' ' && s[newY][newX] != id) {
                return false;
            }
        }
    }
    return true;
}

void screen::left() {
    std::lock_guard<std::mutex> movement(movementMutex);
    std::lock_guard<std::mutex> lock(screenMutex);

    if (!canMove(1))
        return;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (x+i>=12||y+j>=12) continue;
            if (s[y + j][x + i] == id) {
                s[y + j][x + i - 1] = id;
                s[y + j][x + i] = ' ';
            }
        }
    }

    x--;
}

void screen::right() {
    std::lock_guard<std::mutex> lock(screenMutex);

    if (!canMove(3))
        return;

    for (int i = 3; i >= 0; i--) {
        for (int j = 0; j < 4; j++) {
            if (x+i<0||y+j>=12) continue;
            if (s[y + j][x + i] == id) {
                s[y + j][x + i + 1] = id;
                s[y + j][x + i] = ' ';
            }
        }
    }

    x++;
}

void screen::down() {
    std::unique_lock<std::mutex> lock(screenMutex);

    if (!canMove(2)) {
        lock.unlock();
        addBlock(rand() % 5 + 1);
        return;
    }

    for (int j = 3; j >= 0; j--) {
        for (int i = 0; i < 4; i++) {
            if (x+i>=12||y+j>=12) continue;
            if (s[y + j][x + i] == id) {
                s[y + j + 1][x + i] = id;
                s[y + j][x + i] = ' ';
            }
        }
    }

    y++;
}

void screen::printScreen() {
    // std::cout << "\033[H\033[J";
    for (int i = -1; i < 13; i++) {
        for (int j = -1; j < 13; j++) {
            if (i == -1 || j == -1 || i == 12 || j == 12) std::cout << "# ";
            else std::cout << s[i][j] << ' ';
        }
        std::cout << '\n';
    }
}
