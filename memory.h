#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include "utils.h"

class GB;
class Video;

class Memory {
    public:
        std::vector<byte> mem;
        Video& video;
        GB& cpu;


        void io_write(size_t addr, byte b);
        byte io_read(size_t addr);

        void write(size_t addr, byte b);

        byte read(size_t n);

        Memory(Video& video, GB& cpu) : video(video), mem(64*1024), cpu(cpu) {}
};


#endif
