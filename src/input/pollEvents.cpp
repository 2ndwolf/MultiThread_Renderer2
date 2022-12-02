// #include <memory>

#include <SDL.h> 

#include "input.h"
#include "fkore.h"
#include "window.h"

namespace FK {
  namespace ORE{

    int pollEvents () {
      SDL_Event event;

      // Increment pressed state, or delete if released during previous frame
      // updateInputStates(Mouse::clicks);
      updateInputStates(FK::ORE::KeyStatesMap::keycodeStates);

      while (SDL_PollEvent(&event)) {
        FK::Window::pollWindowEvents(event);
        switch (event.type) { 

        case SDL_WINDOWEVENT: {
        }
        case SDL_QUIT: 
          // return 1;

        case SDL_KEYDOWN:
          KeyStatesMap::keycodeStates.insert({event.key.keysym.scancode, ENUM::KeyStates::DOWN}); 
          break;

        case SDL_KEYUP:
          KeyStatesMap::keycodeStates[event.key.keysym.scancode] = ENUM::KeyStates::UP;
          break;

        case SDL_MOUSEBUTTONDOWN:
          Mouse::clicks[event.button.button] = ENUM::KeyStates::DOWN;
          break;

        case SDL_MOUSEBUTTONUP:
          Mouse::clicks[event.button.button] = ENUM::KeyStates::UP;
          break;

        case SDL_MOUSEMOTION:
          FK::Mouse::position.x = event.motion.x;
          FK::Mouse::position.y = event.motion.y;
          break;
        }
      }

      return 0;
    }

    void updateInputStates(std::map<SDL_Scancode, ENUM::KeyStates> &inputStates){
      std::map<SDL_Scancode, ENUM::KeyStates>::iterator it = inputStates.begin();
      while (it != inputStates.end()){

        if(it -> second == ENUM::KeyStates::DOWN){
          it->second = ENUM::KeyStates::HELD;

        } else if (it -> second == ENUM::KeyStates::UP){
          it = inputStates.erase(it);
          continue;
        }

        it++;

      }

      std::map<int, ENUM::KeyStates>::iterator mit = FK::Mouse::clicks.begin();
      while (mit != FK::Mouse::clicks.end()){

        if(mit -> second == ENUM::KeyStates::DOWN){
          mit->second = ENUM::KeyStates::HELD;

        } else if (mit -> second == ENUM::KeyStates::UP){
          mit->second = ENUM::KeyStates::INACTIVE;
        }

        mit++;

      }
    }
  }
}
