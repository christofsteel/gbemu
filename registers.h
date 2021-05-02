#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstddef>
#include <iomanip>
#include "utils.h"

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
            oss << "A: " << HEX(this->a,2) << " " 
                << "F: " << HEX(this->f,2) << " " 
                << "B: " << HEX(this->b,2) << " " 
                << "C: " << HEX(this->c,2) << " " 
                << "D: " << HEX(this->d,2) << " " 
                << "E: " << HEX(this->e,2) << " " 
                << "H: " << HEX(this->h,2) << " " 
                << "L: " << HEX(this->l,2) << " " 
                << "SP: " << HEX(this->sp,4) << " "
                << "PC: " << HEX(this->pc,4) << " ";
            if (get_z()) { oss << "Z"; } else { oss << "-"; }
            if (get_n()) { oss << "N"; } else { oss << "-"; }
            if (get_h()) { oss << "H"; } else { oss << "-"; }
            if (get_c()) { oss << "C"; } else { oss << "-"; }
            return oss.str();
        }
};
#endif
