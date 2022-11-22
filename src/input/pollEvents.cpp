#include <iostream>

#include <SDL.h> 

#include "input.h"

namespace Input {

  int pollEvents () {
    SDL_Event event;

    // Increment pressed state, or delete if released during previous frame
    // updateInputStates(Mouse::clicks);
    updateInputStates(KeyStatesMap::keycodeStates);

    while (SDL_PollEvent(&event)) { 
      switch (event.type) { 

      case SDL_WINDOWEVENT:

        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
            // logFileStderr("MESSAGE:Resizing window...\n");
            // resizeWindow(event.window.data1, event.window.data2);
        }
          
        break;
      case SDL_QUIT: 
        return 1;

      case SDL_KEYDOWN:
        KeyStatesMap::keycodeStates.insert({event.key.keysym.scancode, Input::KeyStates::DOWN}); 
        break;

      case SDL_KEYUP:
        KeyStatesMap::keycodeStates[event.key.keysym.scancode] = Input::KeyStates::UP;
        break;

      // case SDL_MOUSEBUTTONDOWN:
      //   switch(event.button.button){
      //     case SDL_BUTTON_LEFT:
      //       Mouse::clicks[SDL_BUTTON_LEFT] =  1;
      //       break;
      //     case SDL_BUTTON_RIGHT:
      //       Mouse::clicks[SDL_BUTTON_RIGHT] =  1;
      //       break;
      //   }

      //   break;

      // case SDL_MOUSEBUTTONUP:
      //   switch(event.button.button){
      //     case SDL_BUTTON_LEFT:
      //       Mouse::clicks[SDL_BUTTON_LEFT] = -1;
      //     break;
      //     case SDL_BUTTON_RIGHT:
      //       Mouse::clicks[SDL_BUTTON_RIGHT] = -1;
      //     break;
      //   }
      //   break;

      // case SDL_MOUSEMOTION:
      //   Mouse::x = event.motion.x;
      //   Mouse::y = event.motion.y;
      //   break;
      }
    }

    // Input::updateMouse();

    return 0;
  }

  void updateInputStates(std::map<SDL_Scancode, KeyStates> &inputStates){
    std::map<SDL_Scancode, KeyStates>::iterator it = inputStates.begin();
    while (it != inputStates.end()){

      if(it -> second == Input::KeyStates::DOWN){
        it->second = Input::KeyStates::HELD;

      } else if (it -> second == Input::KeyStates::UP){
        it = inputStates.erase(it);
        continue;
      }

      it++;

    }
  }

  //bool keypressed, keyheld, keyup, keydown
  bool keyPressed(std::string key){
    for(int i = 0; i < Input::Keybinds::keys[key].size(); i++){
      if(Input::KeyStatesMap::keycodeStates[Input::Keybinds::keys[key][i]] > Input::KeyStates::INACTIVE) return true;
    }
    return false;
  }

  bool keyHeld(std::string key){
    for(int i = 0; i < Input::Keybinds::keys[key].size(); i++){
      if(Input::KeyStatesMap::keycodeStates[Input::Keybinds::keys[key][i]] == Input::KeyStates::HELD) return true;
    }
    return false;
  }

  bool keyUp(std::string key){
    for(int i = 0; i < Input::Keybinds::keys[key].size(); i++){
      if(Input::KeyStatesMap::keycodeStates[Input::Keybinds::keys[key][i]] == Input::KeyStates::UP) return true;
    }
    return false;
  }

  bool keyDown(std::string key){
    for(int i = 0; i < Input::Keybinds::keys[key].size(); i++){
      if(Input::KeyStatesMap::keycodeStates[Input::Keybinds::keys[key][i]] == Input::KeyStates::DOWN) return true;
    }
    return false;
  }
}
