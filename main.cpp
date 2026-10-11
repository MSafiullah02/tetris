#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include "screen.h"

termios originalTermios;
int originalFlags;

volatile std::sig_atomic_t stopRequested = 0;

void handleSigint(int signal) {
    stopRequested = 1;
}

void screenManager() {
    while (game) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::lock_guard<std::mutex> lock(screenMutex);
        //std::cout << "\033[2J\033[H";
        std::cout << "\033[H\033[J";
        Screen.printScreen();
        std::cout.flush();
    }
}

void progress() {
    int delay = 1000;
    while (game) {
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        // std::lock_guard<std::mutex> lock(screenMutex);
        Screen.down();
        if (delay > 500) {
            delay--;
        }
    }
}

void setupKeyboard() {
    tcgetattr(STDIN_FILENO, &originalTermios);
    originalFlags = fcntl(STDIN_FILENO, F_GETFL, 0);

    termios t = originalTermios;
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = 1;
    t.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &t);
    fcntl(STDIN_FILENO, F_SETFL, originalFlags | O_NONBLOCK);
}

void restoreKeyboard() {
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
    fcntl(STDIN_FILENO, F_SETFL, originalFlags);
}

char getInput() {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0)
        return c;

    return 0;
}

int main() {
    std::signal(SIGINT, handleSigint);
    std::system("tput smcup");
    std::cout << "\033[?1049h";  // Enter alternate screen
    std::cout << "\033[2J\033[H";
    std::cout.flush();
    Screen.addBlock(1);

    std::thread scr(screenManager);
    std::thread prog(progress);

    setupKeyboard();

    while (game && !stopRequested) {
        char input = getInput();

        if (input == 'a') {
            Screen.left();
        }
        else if (input == 'd') {
            Screen.right();
        }
        else if (input == 's') {
            Screen.down();
        }
        else if (input == '1') {
            Screen.addBlock(1);
        }
        else if (input == '2') {
            Screen.addBlock(2);
        }
        else if (input == '3') {
            Screen.addBlock(3);
        }
        else if (input == '4') {
            Screen.addBlock(4);
        }
        else if (input == '5') {
            Screen.addBlock(5);
        }
        else if (input == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    
    game = false;
    scr.join();
    prog.join();
    restoreKeyboard();
    std::cout << "\033[?1049l";
    std::system("tput rmcup");
    std::cout << "Game Over!" << std::endl;
}
