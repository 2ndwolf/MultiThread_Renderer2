#ifndef _init_mtr_h_
#define _init_mtr_h_



#if defined(TSR_ALL) || defined(AISAF_ALL)
// #ifdef TSR_ALL | AISAF_ALL
  #define AISAF
  #define TSR_Preferences
  #define TSR
  #define TSR_TTF
  #define TSR_primitives
  #define TSR_Video
  #define TSR_Audio
  #define TSR_Input
#endif

#include <SDL.h>
#include <functional>

#ifdef AISAF
  #include "audio.h"
  #include "input.h"
#endif
#ifdef TSR_Preferences
  #include "FKpreferences.h"
#endif
#ifdef TSR
  #include "fkore.h"
  #include "information.h"
  #include "renderable.h"
  #include "window.h"
  #include "AT.h"
#endif
#ifdef TSR_primitives
  #include "TSRPrimitives.h"
#endif

namespace FK::INIT {
  int init(){

    // SDL_SetMainReady();

    if (SDL_Init(
      SDL_INIT_TIMER | //: timer subsystem
      #ifdef TSR_Video
        SDL_INIT_VIDEO |//: video subsystem; automatically initializes the events subsystem
      #endif
      #ifdef AISAF
        SDL_INIT_AUDIO |//: audio subsystem
        SDL_INIT_JOYSTICK | //: joystick subsystem; automatically initializes the events subsystem
        //SDL_INIT_HAPTIC |//: haptic (force feedback) subsystem
        // SDL_INIT_GAMECONTROLLER |//: controller subsystem; automatically initializes the joystick subsystem
      #endif
      SDL_INIT_EVENTS //: events subsystem
      ) != 0) { 
      printf("error initializing SDL: %s\n", SDL_GetError()); 
      return 1;
    }

    #ifdef TSR_Video
      // SDLA::Rendering();
    #endif

    #ifdef TSR_Preferences
      FK::ORE::parseIni("assets/uPref.ini");
      // 2
      #ifdef TSR_Input
        FK::ORE::initKeyBinds(FK::ORE::uPreferences::uPrefs.keys);
        // 3
      #endif
    #endif

    #ifdef TSR_Audio
      FK::ORE::init();
      // 4
    #endif

    #ifdef TSR_TTF
      if (TTF_Init() == -1){
        printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
        return 5;
      }
    #endif

    return 0;
  };

  int close(){
    SDL_Quit();
    // SDLA::Rendering
    // SDLA::render->~Rendering();
    return 0;
  };
}

std::function<int()> FK_Init  = FK::INIT::init;
std::function<int()> FK_Close = FK::INIT::close;

#endif