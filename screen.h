#ifndef SCREEN_H
#define SCREEN_H

#include "block.h"
#include <mutex>
#include <atomic>

extern std::mutex screenMutex;
extern std::atomic<bool> game;

class screen {
private:
    char s[12][12];
    char id = ' ';
    short x = 0;
    short y = 0;

    bool canMove(char m);

public:
    screen();

    void addBlock(char n);
    void left();
    void right();
    void down();
    void printScreen();
};

extern screen Screen;

#endif
