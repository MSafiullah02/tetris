#include "block.h"

#define o ' '

char block::k = 'a' - 1;

block::block(const char data[4][4], char id) : id(id) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b[i][j] = data[i][j];
        }
    }
}

void block::rotate() {
    char t[4][4];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            t[i][j] = b[3 - j][i];
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            b[i][j] = t[i][j];
        }
    }
}

block block::getBlock(char n) {
    if (k == 'Z') {
        k = 'a';
    }
    else if (k == 'z') {
        k = 'A';
    }
    else {
        k++;
    }

    switch (n) {
        case 1: {
            char data[4][4] = {
                {o, o, o, o},
                {k, k, k, k},
                {o, o, o, o},
                {o, o, o, o}
            };

            return block(data, k);
        }

        case 2: {
            char data[4][4] = {
                {o, o, o, o},
                {o, k, k, o},
                {o, k, k, o},
                {o, o, o, o}
            };

            return block(data, k);
        }

        case 3: {
            char data[4][4] = {
                {o, o, o, o},
                {k, k, k, o},
                {o, k, o, o},
                {o, o, o, o}
            };

            return block(data, k);
        }

        case 4: {
            char data[4][4] = {
                {o, o, o, o},
                {k, k, k, o},
                {o, o, k, o},
                {o, o, o, o}
            };

            return block(data, k);
        }

        case 5: {
            char data[4][4] = {
                {o, o, o, o},
                {o, o, k, o},
                {k, k, k, o},
                {o, o, o, o}
            };

            return block(data, k);
        }
    }

    char data[4][4] = {
        {o, o, o, o},
        {o, o, o, o},
        {o, o, o, o},
        {o, o, o, o}
    };

    return block(data, o);
}
