#ifndef _FKORE_h_
#define _FKORE_h_

#include <string>
#include <map>
#include <memory>
#include <thread>

#include <SDL.h>
#include <SDL_image.h> 
#include <SDL_ttf.h>

// #include "info.h"
// #include "window.h"


namespace FKORE {
  // class Rendering;

  struct SDLSurface{
    std::string fileName;

    SDL_Surface* sur;
    int useCount = 0;
  };

  struct SDLFont{
    std::string fileName;
    TTF_Font* font;
    int size;
  };

  inline std::map<std::string, SDLSurface> surfaces;
  inline std::map<std::string, SDLFont> fonts;


  // inline static Rendering* render;

  // static void pollWindowEvents(SDL_Event event){
  //   std::map<std::string, std::shared_ptr<FK::Window>>::iterator it;
  //   if(windows.size() != 0) {/* windows[currentWindow]->display(); */
  //     for(it = windows.begin(); it != windows.end(); it++){
  //       it->second->pollEvents(event);
  //     }
  //   }
  // }


}




#endif