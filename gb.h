#include <SDL2/SDL.h>
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>
#include <thread>
#include <chrono>
#include <sstream>

typedef uint8_t byte;

class Registers {
    public:
        union {
            struct { byte f; byte a; };
            uint16_t af;
        };
        union {
            struct { byte c; byte b; };
            uint16_t bc;
        };
        union {
            struct { byte e; byte d; };
            uint16_t de;
        };
        union {
            struct { byte l; byte h; };
            uint16_t hl;
        };
        uint16_t sp;
        uint16_t pc;

        // f register:
        // XXXXZNHC
        //     8421
        void set_f(bool b, int flag) {
            if(b){
                this->f = this->f | flag;
            } else {
                this->f = this->f & (15 - flag);
            }
        }
        bool get_f(byte mask) {
            return (this->f & mask) != 0;
        }
        void set_z(bool b) { set_f(b, 8); }
        void set_n(bool b) { set_f(b, 4); }
        void set_h(bool b) { set_f(b, 2); }
        void set_c(bool b) { set_f(b, 1); }
        bool get_z() { return get_f(8); }
        bool get_n() { return get_f(4); }
        bool get_h() { return get_f(2); }
        bool get_c() { return get_f(1); }

        std::string to_dbg_str() {
            std::ostringstream oss;
            oss << "A: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->a << " " 
                << "F: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->f << " " 
                << "B: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->b << " " 
                << "C: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->c << " " 
                << "D: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->d << " " 
                << "E: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->e << " " 
                << "H: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->h << " " 
                << "L: " << std::hex << std::setw(2) << std::setfill('0') << (unsigned int) this->l << " " 
                << "SP: " << std::hex << std::setw(4) << std::setfill('0') << (unsigned int) this->sp << " "
                << "PC: " << std::hex << std::setw(4) << std::setfill('0') << (unsigned int) this->pc << " ";
            if (get_z()) { oss << "Z"; } else { oss << "-"; }
            if (get_n()) { oss << "N"; } else { oss << "-"; }
            if (get_h()) { oss << "H"; } else { oss << "-"; }
            if (get_c()) { oss << "C"; } else { oss << "-"; }
            return oss.str();
        }
};
class Video {
    public:
        SDL_Window* win;
        void createWindow();
        void set_lcdc(byte) {
            
        }
        Video();
        ~Video();
};

class Memory {
    public:
        std::vector<byte> mem;
        Video& video;

        void write(size_t addr, byte b) {
            if(addr == 0xff40) {
                int i;
                std::cin >> i;
            }
            std::cout << "Write 0x" << std::hex << addr << ": 0x" << std::hex << std::setw(2) << std::setfill ('0') << (unsigned int) b << std::endl;
            mem[addr] = b;
        }

        byte read(size_t n) {
            return mem[n];
        }

        Memory(Video& video) : video(video), mem(64*1024) {}
};


class GB {
    public:
        Registers regs;
        Memory mmu;
        Video video;
        int ints;
        size_t clock = 238;
        std::time_t cycletime;
        std::string filename;
        GB(std::string filename) : filename(filename), regs(), video(), mmu(video), ints(0) {
            std::ifstream filestream(filename, std::ios::binary);
            filestream.read(reinterpret_cast<char*>(mmu.mem.data()), 32 * 1024);
            filestream.close();
            this->regs.a = 0x11;
            this->regs.f = 0x80;
            this->regs.d = 0xff;
            this->regs.e = 0x56;
            this->regs.sp = 0xfffe;
            this->regs.pc = 0x0100;
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

        bool step();

        void sleep_cycles(size_t n) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(4 * n * this->clock));
        }

        void nop() {
            this->sleep_cycles(1);
        }

        void jp(uint16_t addr) {
            this->regs.pc = addr;
            this->sleep_cycles(4);
        }

        void jrnz(byte offset) {
            if (!this->regs.get_z()) {
                this->regs.pc = this->regs.pc + static_cast<std::make_signed_t<int8_t>>(offset);
                this->sleep_cycles(1);
            }
            this->sleep_cycles(2);
        }

        void opxor(byte a, byte b) {
            this->regs.a = a ^ b;
            this->regs.set_z(this->regs.a == 0);
            this->regs.set_n(false);
            this->regs.set_h(false);
            this->regs.set_c(false);
            this->sleep_cycles(1);
        }
        void ld_mem(uint16_t dst, byte val) {
            this->mmu.write(dst, val);            
            this->sleep_cycles(2);
        }
        void ld_reg8(uint8_t* ref, uint8_t val) {
            (*ref) = val;
            this->sleep_cycles(2);
        }
        void ld_reg16(uint16_t* ref, uint16_t val) {
            (*ref) = val;
            this->sleep_cycles(3);
        }
        void dec(uint8_t* reg) {
            this->regs.set_h((*reg) & 0xf - (1 & 0xf) < 0);
            this->regs.set_n(true);
            (*reg)--;
            this->regs.set_z((*reg) == 0);
            this->sleep_cycles(1);
        }

        void di() {
            this->ints = false;
            this->sleep_cycles(1);
        }

        void cp(byte n) {
            this->regs.set_n(true);
            this->regs.set_z(this->regs.a == n);
            this->regs.set_c(this->regs.a < n);
            this->sleep_cycles(2);
        }
};
