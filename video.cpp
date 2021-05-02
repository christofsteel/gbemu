#include "video.h"

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
    this->createWindow();
    //SDL_Delay(200);
}

Video::~Video() {
    //SDL_DestroyWindow(this->win);
}
void Video::blank() {
    __TODO__("blank")
}

void Video::set_lcdc(byte b) {
    if(b & 1 != 0) {
        // TODO
    } else {
        this->blank();
    }
    this->obj_enable = (b & 2) != 0;
    this->obj_size = (b & 4) != 0; 
    this->tilemap_area = (b & 8) != 0;
    this->tiledata_area = (b & 16) != 0;
    this->window_enabled = (b & 32) != 0;
    this->window_tilemap_area = (b & 64) != 0;
    this->enabled = (b & 128) != 0;
}
