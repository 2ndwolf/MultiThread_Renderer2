#include <memory>

#include <SDL.h> 

#include "input.h"
#include "rendering.h"

namespace Input {

  int pollEvents () {
    SDL_Event event;

    // Increment pressed state, or delete if released during previous frame
    // updateInputStates(Mouse::clicks);
    updateInputStates(KeyStatesMap::keycodeStates);

    while (SDL_PollEvent(&event)) {
      SDLA::Rendering::pollWindowEvents(event);
      switch (event.type) { 

      case SDL_WINDOWEVENT: {
      }
      case SDL_QUIT: 
        return 1;

      case SDL_KEYDOWN:
        KeyStatesMap::keycodeStates.insert({event.key.keysym.scancode, Input::KeyStates::DOWN}); 
        break;

      case SDL_KEYUP:
        KeyStatesMap::keycodeStates[event.key.keysym.scancode] = Input::KeyStates::UP;
        break;

      case SDL_MOUSEBUTTONDOWN:
        Mouse::clicks[event.button.button] = Input::KeyStates::DOWN;
        break;

      case SDL_MOUSEBUTTONUP:
        Mouse::clicks[event.button.button] = Input::KeyStates::UP;
        break;

      case SDL_MOUSEMOTION:
        Mouse::mousePos.x = event.motion.x;
        Mouse::mousePos.y = event.motion.y;
        break;
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

    std::map<int, KeyStates>::iterator mit = Mouse::clicks.begin();
    while (mit != Mouse::clicks.end()){

      if(mit -> second == Input::KeyStates::DOWN){
        mit->second = Input::KeyStates::HELD;

      } else if (mit -> second == Input::KeyStates::UP){
        mit->second = Input::KeyStates::INACTIVE;
      }

      mit++;

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
