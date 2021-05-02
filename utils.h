#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstddef>

#define __TODO__(x) std::cout<<"TODO: " << x <<std::endl;exit(1);
#define __DIE__(x) std::cout<<"ERROR: " << x <<std::endl;exit(1);
#define HEX(x,y) "0x" << std::hex << std::setw(y) << std::setfill('0') << (int) x

typedef uint8_t byte;

#endif
