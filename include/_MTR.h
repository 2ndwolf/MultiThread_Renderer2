#ifndef _init_mtr_h_
#define _init_mtr_h_

#ifdef MTR_ALL
  #define MTR_TTF
  #define MTR_Audio
  #define MTR_Input 
  #define MTR_Preferences
  #define MTR_Video
#endif

#include <functional>

#ifdef MTR_Audio
  #include "audio.h"
#endif
#ifdef MTR_Input
  #include "input.h"
#endif
#ifdef MTR_Preferences
  #include "preferences.h"
#endif
#ifdef MTR_Video
  #include "rendering.h"
#endif

#include "primitives.h"

namespace SDLA {
  int init(){
    if (SDL_Init(
      SDL_INIT_TIMER | //: timer subsystem
      #ifdef MTR_Video
        SDL_INIT_VIDEO |//: video subsystem; automatically initializes the events subsystem
      #endif
      #ifdef MTR_Audio
        SDL_INIT_AUDIO |//: audio subsystem
      #endif
      #ifdef MTR_Input
        SDL_INIT_JOYSTICK //: joystick subsystem; automatically initializes the events subsystem
        //SDL_INIT_HAPTIC |//: haptic (force feedback) subsystem
        // SDL_INIT_GAMECONTROLLER |//: controller subsystem; automatically initializes the joystick subsystem
      #endif
      // SDL_INIT_EVENTS //: events subsystem
      ) != 0) { 
      printf("error initializing SDL: %s\n", SDL_GetError()); 
      return 1;
    }

    #ifdef MTR_Video
      SDLA::Rendering();
    #endif

    #ifdef MTR_Preferences
      Preferences::parseIni("assets/uPref.ini");
      // 2
      #ifdef MTR_Input
        Input::initKeyBinds(Preferences::uPreferences::uPrefs.keys);
        // 3
      #endif
    #endif

    #ifdef MTR_Audio
      Audio::init();
      // 4
    #endif

    #ifdef MTR_TTF
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
    SDLA::render->~Rendering();
    return 0;
  };
}

std::function<int()> MTR_Init  = SDLA::init;
std::function<int()> MTR_Close = SDLA::close;

#endif