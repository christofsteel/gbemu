#include <iostream>
#include <iomanip>
#include <bitset>
#include "gb.h"

void Video::createWindow() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "error initializing SDL: " << SDL_GetError() << std::endl;
    }
    this->win = SDL_CreateWindow("GB", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 160, 144, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(this-> win);
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(this->win);
}

Video::Video() {
    //this->createWindow();
    //SDL_Delay(200);
}

Video::~Video() {
    //SDL_DestroyWindow(this->win);
}

void GB::gameloop() {
    std::cout << this->filename << std::endl;
}

std::string GB::disassemble_addr(size_t addr, byte opcode, std::vector<byte> args) {
    std::ostringstream oss;
    oss << std::hex << std::setw(4) << std::setfill ('0') << (int) addr << " | " << this->opcode_names[opcode];
    for (auto c : args) {
        oss << " " << std::hex << std::setw(2) << std::setfill ('0') << (int) c;
    }
    return oss.str();
}


int main(int argc, char* args[]) {
    GB gb{"tetris.gb"};
    gb.gameloop();
    int skip = 0;
    while (gb.step()) {
        if(skip == 0) {
            std::cin >> skip;
        } else {
            skip--;
        }
    };
    return 0;
}

bool GB::step() {

    this->checkvideo();

    uint16_t addr = this->regs.pc;
    byte opcode = this->mmu.read(addr);
    size_t arity = this->arity_table[opcode];
    std::vector<byte> args(this->mmu.mem.begin() + addr + 1, this->mmu.mem.begin() + addr + 1 + (arity - 1));
    std::cout << this->disassemble_addr(addr, opcode, args) << std::endl 
              << this->regs.to_dbg_str() << std::endl;

    this->regs.pc += arity;

    switch(opcode) {
        case 0x00:
            this->nop();
            break;
        case 0x05:
            this->dec(&(this->regs.b));
            break;
        case 0x06:
            this->ld_reg8(&(this->regs.b), args[0]);
            break;
        case 0x0d:
            this->dec(&(this->regs.c));
            break;
        case 0x0e:
            this->ld_reg8(&(this->regs.c), args[0]);
            break;
        case 0x20:
            this->jrnz(args[0]);
            break;
        case 0x21:
            this->ld_reg16(&(this->regs.hl), args[1] << 8 | args[0]);
            break;
        case 0x32:
            this->ld_mem(this->regs.hl, this->regs.a);
            this->regs.hl--;
            break;
        case 0x3e:
            this->ld_reg8(&(this->regs.a), args[0]);
            break;
        case 0xaf:
            this->opxor(this->regs.a, this->regs.a);
            return true;
            break;
        case 0xc3:
            this->jp(args[1] << 8 | args[0]);
            break;
        case 0xe0:
            this->ld_mem(0xff00 | args[0], this->regs.a);
            this->sleep_cycles(1);
            break;
        case 0xf0:
            this->ld_reg8(&(this->regs.a), this->mmu.read(0xff00 | args[0]));
            this->sleep_cycles(1);
            break;
        case 0xf3:
            this->di();
            break;
        case 0xfe:
            this->cp(args[0]);
            break;
        default:
            std::cout << "Not implemented" << std::endl;
            return false;
    }
    return true;
}
