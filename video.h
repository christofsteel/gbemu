#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>
#include "utils.h"

class Video {
    public:
        SDL_Window* win;

        bool obj_enable;
        bool obj_size;
        bool tilemap_area;
        bool tiledata_area;
        bool window_tilemap_area;
        bool window_enabled;
        bool enabled;
        bool scy;
        bool scx;
        
        void blank();
        void createWindow();
        void set_lcdc(byte b);
        void set_scy(byte b) {
            this->scy = b;
        }
        void set_scx(byte b) {
            this->scx = b;
        }
        Video();
        ~Video();
};

#endif
