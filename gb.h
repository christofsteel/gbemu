#ifndef GB_H
#define GB_H

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

#include "registers.h"
#include "memory.h"
#include "video.h"

class GB {
    public:
        Registers regs;
        Memory mmu;
        Video video;
        bool ints;
        long long instr_count;
        int threecycle;
        byte timer;
        byte timer_start;
        bool timer_enabled;
        int timer_clock;
        std::string filename;
        bool evblank;
        bool elcd_stat;
        bool etimer;
        bool eserial;
        bool ejoypad;
        bool ivblank;
        bool ilcd_stat;
        bool itimer;
        bool iserial;
        bool ijoypad;
        GB(std::string filename) : filename(filename), regs(), video(), mmu(video, *this), ints(true) {
            std::ifstream filestream(filename, std::ios::binary);
            filestream.read(reinterpret_cast<char*>(this->mmu.mem.data()), 32 * 1024);
            filestream.close();
            this->regs.a = 0x11;
            this->regs.f = 0x80;
            this->regs.d = 0xff;
            this->regs.e = 0x56;
            this->regs.sp = 0xfffe;
            this->regs.pc = 0x0100;
            this->mmu.write(0xff05, 0x00);
            this->mmu.write(0xff06, 0x00);
            this->mmu.write(0xff07, 0x00);
            this->mmu.write(0xff10, 0x80);
            this->mmu.write(0xff11, 0xbf);
            this->mmu.write(0xff12, 0x3f);
            this->mmu.write(0xff14, 0xbf);
            this->mmu.write(0xff16, 0x3f);
            this->mmu.write(0xff17, 0x00);
            this->mmu.write(0xff19, 0xbf);
            this->mmu.write(0xff1a, 0x7a);
            this->mmu.write(0xff1b, 0xff);
            this->mmu.write(0xff1c, 0x9f);
            this->mmu.write(0xff1e, 0x8f);
            this->mmu.write(0xff20, 0xff);
            this->mmu.write(0xff21, 0x00);
            this->mmu.write(0xff22, 0x00);
            this->mmu.write(0xff23, 0xbf);
            this->mmu.write(0xff24, 0x77);
            this->mmu.write(0xff25, 0xf3);
            this->mmu.write(0xff26, 0xf1);
            this->mmu.write(0xff40, 0x91);
            this->mmu.write(0xff42, 0x00);
            this->mmu.write(0xff43, 0x00);
            this->mmu.write(0xff45, 0x0);
            this->mmu.write(0xff47, 0xfc);
            this->mmu.write(0xff48, 0xff);
            this->mmu.write(0xff49, 0xff);
            this->mmu.write(0xff4a, 0x00);
            this->mmu.write(0xff4b, 0x00);
            this->mmu.write(0xffff, 0x00);


            this->ivblank = false;
            this->ilcd_stat = false;
            this->itimer = false;
            this->iserial = false;
            this->ijoypad = false;
        }
        std::string disassemble_addr(size_t addr, byte opode, std::vector<byte> args);
        void gameloop();

        std::vector<size_t> arity_table {
         // 0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f 
            1, 3, 1, 1, 1, 1, 2, 1, 3, 1, 1, 1, 1, 1, 2, 1, // 0
            2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 1
            2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 2
            2, 3, 1, 1, 1, 1, 2, 1, 2, 1, 1, 1, 1, 1, 2, 1, // 3
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 4
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 5
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 6
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 7
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 8
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // 9
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // a
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, // b
            1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1, // c
            1, 1, 3, 0, 3, 1, 2, 1, 1, 1, 3, 0, 3, 0, 2, 1, // d
            2, 1, 1, 0, 0, 1, 2, 1, 2, 1, 3, 0, 0, 0, 2, 1, // e
            2, 1, 1, 1, 0, 1, 2, 1, 2, 1, 3, 1, 0, 0, 2, 1, // f
        };

        std::vector<std::string> opcode_names {
        //   0      1      2      3      4      5      6      7      8      9      a      b      c      d      e      f 
            "NOP", "?01", "?02", "?03", "?03", "DEC B", "LD B, d8", "?07", "?08", "?09", "?0a", "?0b", "?0c", "DEC C", "LD C, d8", "?0f", // 0
            "?10", "?11", "?12", "?13", "?14", "?15", "?16", "?17", "?18", "?19", "?1a", "?1b", "?1c", "?1d", "?1e", "?1f", // 1
            "JR NZ, s8", "LD HL, d16", "?22", "?23", "?24", "?25", "?26", "?27", "?28", "?29", "?2a", "?2b", "?2c", "?2d", "?2e", "?2f", // 2
            "?30", "?31", "LD (HL-) A", "?33", "?34", "?35", "?36", "?37", "?38", "?39", "?3a", "?3b", "?3c", "?3d", "LD A,u8", "?3f", // 3
            "?40", "?41", "?42", "?43", "?44", "?45", "?46", "?47", "?48", "?49", "?4a", "?4b", "?4c", "?4d", "?4e", "?4f", // 4
            "?50", "?51", "?52", "?53", "?54", "?55", "?56", "?57", "?58", "?59", "?5a", "?5b", "?5c", "?5d", "?5e", "?5f", // 5
            "?60", "?61", "?62", "?63", "?64", "?65", "?66", "?67", "?68", "?69", "?6a", "?6b", "?6c", "?6d", "?6e", "?6f", // 6
            "?70", "?71", "?72", "?73", "?74", "?75", "?76", "?77", "?78", "?79", "?7a", "?7b", "?7c", "?7d", "?7e", "?7f", // 7
            "?80", "?81", "?82", "?83", "?84", "?85", "?86", "?87", "?88", "?89", "?8a", "?8b", "?8c", "?8d", "?8e", "?8f", // 8
            "?90", "?91", "?92", "?93", "?94", "?95", "?96", "?97", "?98", "?99", "?9a", "?9b", "?9c", "?9d", "?9e", "?9f", // 9
            "?a0", "?a1", "?a2", "?a3", "?a4", "?a5", "?a6", "?a7", "?a8", "?a9", "?aa", "?ab", "?ac", "?ad", "?ae", "XOR A", // a
            "?b0", "?b1", "?b2", "?b3", "?b4", "?b5", "?b6", "?b7", "?b8", "?b9", "?ba", "?bb", "?bc", "?bd", "?be", "?bf", // b
            "?c0", "?c1", "?c2", "JP u16", "?c4", "?c5", "?c6", "?c7", "?c8", "?c9", "?ca", "?cb", "?cc", "?cd", "?ce", "?cf", // c
            "?d0", "?d1", "?d2", "?d3", "?d4", "?d5", "?d6", "?d7", "?d8", "?d9", "?da", "?db", "?dc", "?dd", "?de", "?df", // d
            "LD (a8), A", "?e1", "?e2", "?e3", "?e4", "?e5", "?e6", "?e7", "?e8", "?e9", "?ea", "?eb", "?ec", "?ed", "?ee", "?ef", // e
            "LD A, (a8)", "?f1", "?f2", "DI", "?f4", "?f5", "?f6", "?f7", "?f8", "?f9", "?fa", "?fb", "?fc", "?fd", "CP d8", "?ff", // f
        };

        void checkvideo();

        int cpustep();

        void set_timer(byte b) {
            this->timer = b;
        }

        void set_timer_start(byte b) {
            this->timer_start = b;
        }

        void set_timer_control(byte b) {
            this->timer_enabled = b & 4 != 0;
            switch(b & 0x03) {
            case 0:
                this->timer_clock = 1024;
                break;
            case 1:
                this->timer_clock = 16;
                break;
            case 2:
                this->timer_clock = 64;
                break;
            case 3:
                this->timer_clock = 256;
                break;
            default:
                __DIE__("Timer fault")
            }
        }

        void set_ints(byte b) {
            this->ivblank = b & 1 != 0;
            this->ilcd_stat = b & 2 != 0;
            this->itimer = b & 4 != 0;
            this->iserial = b & 8 != 0;
            this->ijoypad = b & 16 != 0;
        }

        void enable_ints(byte b) {
            this->evblank = b & 1 == 0;
            this->elcd_stat = b & 2 == 0;
            this->etimer = b & 4 == 0;
            this->eserial = b & 8 == 0;
            this->ejoypad = b & 16 == 0;
        }

        size_t nop() {
            return 1;
        }

        size_t jp(uint16_t addr) {
            this->regs.pc = addr;
            return 4;
        }

        size_t jrnz(byte offset) {
            if (!this->regs.get_z()) {
                this->regs.pc = this->regs.pc + static_cast<std::make_signed_t<int8_t>>(offset);
                return 1;
            }
            return 2;
        }

        size_t opxor(byte a, byte b) {
            this->regs.a = a ^ b;
            this->regs.set_z(this->regs.a == 0);
            this->regs.set_n(false);
            this->regs.set_h(false);
            this->regs.set_c(false);
            return 1;
        }
        size_t ld_mem(uint16_t dst, byte val) {
            this->mmu.write(dst, val);            
            return 2;
        }
        size_t ld_reg8(uint8_t* ref, uint8_t val) {
            (*ref) = val;
            return 2;
        }
        size_t ld_reg16(uint16_t* ref, uint16_t val) {
            (*ref) = val;
            return 3;
        }
        size_t dec(uint8_t* reg) {
            this->regs.set_h((*reg) & 0xf - (1 & 0xf) < 0);
            this->regs.set_n(true);
            (*reg)--;
            this->regs.set_z((*reg) == 0);
            return 1;
        }

        size_t di() {
            this->ints = false;
            return 1;
        }

        size_t cp(byte n) {
            this->regs.set_n(true);
            this->regs.set_z(this->regs.a == n);
            this->regs.set_c(this->regs.a < n);
            return 2;
        }
};

#endif
