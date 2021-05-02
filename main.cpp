#include "gb.h"


int main(int argc, char* args[]) {
    GB gb{"tetris.gb"};
    gb.instr_count=0;
    gb.gameloop();
    int skip = 0;
    return 0;
}
