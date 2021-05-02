#include <iostream>
#include <iomanip>
#include <bitset>
#include "gb.h"


void GB::gameloop() {
    this->threecycle=0;
    bool running = true;
    while (true) {
        // GB Clockspeed = 4.194304 MHz
        // VSync = 59.73 Hz ~ emulated 59.7
        // ~70255/4 instructions per frame
        // ~17564
        this->checkvideo();
        int cycles = 17564; 
        while (cycles >= 0) {
            int cycles_used = this->cpustep();
            cycles = cycles - cycles_used;
        }
    };
}

std::string GB::disassemble_addr(size_t addr, byte opcode, std::vector<byte> args) {
    std::ostringstream oss;
    oss << HEX(addr,4) << " | " << this->opcode_names[opcode];
    for (auto c : args) {
        oss << " " << HEX(c,2);
    }
    return oss.str();
}



void GB::checkvideo() {
    if (this->threecycle == 3) {
        SDL_Delay(16);
        this->threecycle = 0;
    } else {
        SDL_Delay(17);
    }
}

int GB::cpustep() {
    this->instr_count++;

    uint16_t addr = this->regs.pc;
    byte opcode = this->mmu.read(addr);
    size_t arity = this->arity_table[opcode];
    std::vector<byte> args(this->mmu.mem.begin() + addr + 1, this->mmu.mem.begin() + addr + 1 + (arity - 1));
    std::cout << this->instr_count << " | " << this->disassemble_addr(addr, opcode, args) << std::endl 
              << this->regs.to_dbg_str() << std::endl;

    this->regs.pc += arity;

    int cycles = 0;

    switch(opcode) {
        case 0x00:
            cycles = this->nop();
            break;
        case 0x05:
            cycles = this->dec(&(this->regs.b));
            break;
        case 0x06:
            cycles = this->ld_reg8(&(this->regs.b), args[0]);
            break;
        case 0x0d:
            cycles = this->dec(&(this->regs.c));
            break;
        case 0x0e:
            cycles = this->ld_reg8(&(this->regs.c), args[0]);
            break;
        case 0x20:
            cycles = this->jrnz(args[0]);
            break;
        case 0x21:
            cycles = this->ld_reg16(&(this->regs.hl), args[1] << 8 | args[0]);
            break;
        case 0x32:
            cycles = this->ld_mem(this->regs.hl, this->regs.a);
            this->regs.hl--;
            break;
        case 0x3e:
            cycles = this->ld_reg8(&(this->regs.a), args[0]);
            break;
        case 0xaf:
            cycles = this->opxor(this->regs.a, this->regs.a);
            break;
        case 0xc3:
            cycles = this->jp(args[1] << 8 | args[0]);
            break;
        case 0xe0:
            cycles = 1+this->ld_mem(0xff00 | args[0], this->regs.a);
            break;
        case 0xf0:
            cycles = 1+this->ld_reg8(&(this->regs.a), this->mmu.read(0xff00 | args[0]));
            break;
        case 0xf3:
            cycles = this->di();
            break;
        case 0xfe:
            cycles = this->cp(args[0]);
            break;
        default:
            __TODO__("Not implemented")
            break;
    }
    return cycles;
}
