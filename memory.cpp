#include "memory.h"
#include "video.h"
#include "gb.h"


void Memory::io_write(size_t addr, byte b) {
    if(addr == 0xff05) {
        this->cpu.set_timer(b);
    } else if(addr == 0xff06) {
        this->cpu.set_timer_start(b);
    } else if(addr == 0xff07) {
        this->cpu.set_timer_control(b);
    } else if(addr >= 0xff10 && addr <=0xff26) {
        std::cout << "TODO: SOUNDFOO Ignored" << std::endl;
    } else if(addr == 0xff40) {
        this->video.set_lcdc(b);
    } else if(addr == 0xff42) {
        this->video.set_scy(b);
    } else if(addr == 0xff43) {
        this->video.set_scx(b);
    } else if (addr == 0xff0f) {
        this->cpu.set_ints(b);
    } else {
        __TODO__("IO Write " << HEX(addr,4) << ":" << HEX(b,2))
    }
}

byte Memory::io_read(size_t addr) {
    __TODO__("IO Read " << HEX(addr,4))
}

void Memory::write(size_t addr, byte b) {
    if(addr < 0x8000) {
        __DIE__("Write access to ROM")
    } else if (addr < 0xA000) {
        __TODO__("Write Video RAM")
    } else if (addr < 0xFF00) {
        this->mem[addr] = b;
    } else if (addr < 0xFF80) {
        this->io_write(addr, b);
    } else if (addr < 0xFFFF) {
        this->mem[addr] = b;
    } else {
        this->cpu.enable_ints(b);
    }
}

byte Memory::read(size_t addr) {
    if(addr < 0x8000) {
        return this->mem[addr];
    } else if (addr < 0xA000) {
        __TODO__("READ Video RAM")
    } else if (addr < 0xFF00) {
        return this->mem[addr];
    } else if (addr < 0xFF80) {
        this->io_read(addr);
    } else if (addr < 0xFFFF) {
        return this->mem[addr];
    } else {
        __TODO__("IE read")
    }
    return 0x00;
}
