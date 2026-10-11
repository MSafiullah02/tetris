#ifndef BLOCK_H
#define BLOCK_H

class block {
public:
    char b[4][4];
    char id;

    static char k;

    block(const char data[4][4], char id);

    void rotate();

    static block getBlock(char n);
};

#endif
