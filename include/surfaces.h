#ifndef _MTR_SURFACES_h_
#define _MTR_SURFACES_h_

#include <string>
#include <map>
#include <memory>
#include <thread>

#include <SDL.h>
#include <SDL_image.h> 
#include <SDL_ttf.h>

#include "defaults.h"

namespace MTR {
  // class Rendering;
  namespace SUR{

  struct SDLSurface{
    std::string fileName;

    SDL_Surface* sur;
    int useCount = 0;

    ~SDLSurface(){
      SDL_FreeSurface(sur);
      delete sur;
    }
  };

  struct Font{
    std::string fileName;
    TTF_Font* font;
    int size;

    ~Font(){
      TTF_CloseFont(font);
      delete font;
    }
  };



  inline std::map<std::string, SDLSurface> surfaces;
  inline std::map<std::string, Font> fonts;

  void loadSurface(std::string fileName, bool keepImgInMemory = false){
      if(!surfaces.count(fileName)){
        SDLSurface newSur;
        newSur.sur = IMG_Load(fileName.c_str());
        if(newSur.sur == NULL) newSur.sur = IMG_Load(Defaults::placeHolder.c_str());
        newSur.fileName = fileName;
        if(keepImgInMemory) newSur.useCount = -1;
        else newSur.useCount = 1;
        surfaces.insert({fileName, newSur});

      } else if (keepImgInMemory){
        surfaces[fileName].useCount = -1;

      } else if (surfaces[fileName].useCount != -1){
        surfaces[fileName].useCount += 1;
      }

    }

  //   void changeSurface(std::string fileN){
  //     if(surfaces[information->fileName].useCount != -1){
  //       surfaces[information->fileName].useCount -= 1;
  //       if(surfaces[information->fileName].useCount == 0) surfaces.erase(information->fileName);
  //   }
    
  //   loadSurface(fileN);
  //   information->fileName = fileN;
  //   // texQueued = true;
  // }

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
}




#endif