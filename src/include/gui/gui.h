#pragma once
#include <iostream>

#include "../main.h"
#ifdef ___NATIVE_GUI___
    #include "ui/mainWindow.h";
#endif


#include "../stack/stack.h"

extern device_aspect DEVICE;

//#define SCROLLBAR_WIDTH 25
#define SCROLLBAR_WIDTH 0 //TODO
#define ADDRESS_BAR_HEIGHT 35

struct browser_state {
    std::string action = "SELECTED_ADDRESSBAR";
};

namespace browser {
    class GUI {
        public:
            int x = 0, y = 0;
            browser_state State;
            SDL_Surface *_browser_surface = NULL;
            #ifndef ___NATIVE_GUI___
                SDL_Surface *_gui_surface = NULL;
                SDL_Surface *_overlay_surface = NULL;
            #endif

            GUI() {
                #ifndef ___NATIVE_GUI___
                    // sdl_helper::init();
                #else
                    uiInitOptions options;
                    memset(&options, 0, sizeof (uiInitOptions));
                    uiInit(&options);
                #endif
            }
            ~GUI() {
                #ifndef ___NATIVE_GUI___
                    // sdl_helper::exit();
                #else
                    uiUninit();
                #endif
            }

            void scroll (int x, int y, int accel = 15) {
                if (y > 0)
                    this->y += 1 * accel;
                else if (y < 0)
                    this->y -= 1 * accel;

                if (this->y >= 0) {
                    this->y = 0;
                };
            }

            void prepareTick() {
                #ifndef ___NATIVE_GUI___
                    // Get Window Size
                    int w, h, window_w, window_h;
                    SDL_GetRendererOutputSize(_renderer, &w, &h);
                    SDL_GetWindowSize(_window, &window_w, &window_h);
                    
                    short scaling;
                    if (w/window_w > 1) scaling = (short)w/window_w;
                    else scaling = 1;

                    DEVICE = {scaling, w, h};

                    SDL_RenderClear(_renderer);
                #endif
                
                #ifndef ___NATIVE_GUI___
                    // Clear surfaces
                    if(this->_gui_surface != NULL)
                        SDL_FreeSurface(this->_gui_surface);
                    this->_gui_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.h, 32, 0, 0, 0, 255);
                    SDL_FillRect(_gui_surface, NULL, SDL_MapRGBA(_gui_surface->format, 102, 51, 153, 255));
                    SDL_SetColorKey(this->_gui_surface, SDL_TRUE, SDL_MapRGBA(this->_gui_surface->format, 102, 51, 153, 255));

                    if(this->_overlay_surface != NULL)
                        SDL_FreeSurface(this->_overlay_surface);
                    this->_overlay_surface = SDL_CreateRGBSurface(0, DEVICE.w, DEVICE.h, 32, 0, 0, 0, 0);
                    SDL_SetColorKey(this->_overlay_surface, SDL_TRUE, SDL_MapRGBA(this->_overlay_surface->format, 0, 0, 0, 255));
                #endif
            }

            bool doTick() {
                #ifndef ___NATIVE_GUI___
                    SDL_Rect screen_pos = {0, 0, DEVICE.w, DEVICE.h};
                    SDL_Rect browser_pos_dst = {0, (ADDRESS_BAR_HEIGHT * DEVICE.scaling) + this->y, DEVICE.w - SCROLLBAR_WIDTH, DEVICE.h};

                    SDL_Texture *browser = SDL_CreateTextureFromSurface(_renderer, this->_browser_surface);
                    SDL_RenderCopy(_renderer, browser, &screen_pos, &browser_pos_dst);
                    SDL_DestroyTexture(browser);
                #endif

                #ifndef ___NATIVE_GUI___
                    SDL_Texture *gui = SDL_CreateTextureFromSurface(_renderer, this->_gui_surface);
                    SDL_RenderCopy(_renderer, gui, &screen_pos, &screen_pos);
                    SDL_DestroyTexture(gui);

                    SDL_Texture *overlay = SDL_CreateTextureFromSurface(_renderer, this->_overlay_surface);
                    SDL_RenderCopy(_renderer, overlay, &screen_pos, &screen_pos);
                    SDL_DestroyTexture(overlay);
                #endif
                
                #ifndef ___NATIVE_GUI___
                    SDL_RenderPresent(_renderer);
                #endif

                return true;
            }
    };
}