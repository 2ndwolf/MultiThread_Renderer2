//=================================
// include guard
#ifndef _input_h_
#define _input_h_

#include <map>
#include <string>

#include "SDL_scancode.h"


// #include "dependencies/entt/entity/registry.hpp"

namespace Input{

  void initKeyBinds();

  // void updateMouse(entt::registry &registry);

  int pollEvents();
  void updateInputStates(std::map<int, int> &inputStates);
  void MusicFun();

  class KeyStates{
    public:
    static std::map<int, int> keycodeStates;
  };

  class Keybinds{
    public:
    static std::map<SDL_Scancode, std::string> keys;
  };

}

#endif