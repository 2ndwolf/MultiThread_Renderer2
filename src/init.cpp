#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "INIT.h"
  
int MTR_init(){

  // SDL_SetMainReady();

  if (SDL_Init(SDL_INIT_EVERYTHING
    // SDL_INIT_TIMER | //: timer subsystem
    // // #ifdef MTR_Video
    //   SDL_INIT_VIDEO |//: video subsystem; automatically initializes the events subsystem
    // // #endif
    // // #ifdef MTR_Audio
    //   SDL_INIT_AUDIO |//: audio subsystem
    // // #endif
    // // #ifdef MTR_Input
    //   SDL_INIT_JOYSTICK |//: joystick subsystem; automatically initializes the events subsystem
    //   //SDL_INIT_HAPTIC |//: haptic (force feedback) subsystem
    //   // SDL_INIT_GAMECONTROLLER |//: controller subsystem; automatically initializes the joystick subsystem
    // // #endif
    // SDL_INIT_EVENTS //: events subsystem
    ) != 0) { 
    // SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
    //   "Error initializing SDL", SDL_GetError(), NULL); 
    return 1;
  }

  if (IMG_Init(IMG_INIT_PNG) == 0) return 1;
  // MOVED TO AISAF
  // #ifdef MTR_Preferences
  //   FK::ORE::parseIni("assets/uPref.ini");
  //   // 2
  //   #ifdef MTR_Input
  //     FK::ORE::initKeyBinds(FK::ORE::uPreferences::uPrefs.keys);
  //     // 3
  //   #endif
  // #endif
  // #ifdef MTR_Audio
  //   FK::ORE::init();
  //   // 4
  // #endif

  // #ifdef MTR_TTF
    if (TTF_Init() == -1){
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
      "Error initializing SDL_ttf", TTF_GetError(), NULL); 
      return 1;
    }
  // #endif

  return 0;
};

int MTR_close(){
  SDL_Quit();

  // TODO :: CLEAN!!!

  return 0;
};
