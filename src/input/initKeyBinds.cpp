
#include <map>
#include <string>
#include <vector>

#include "SDL_scancode.h"

#include "input.h"

namespace Input{
  // Keybinds::keys;

  void initKeyBinds(std::map<std::string, std::vector<SDL_Scancode>> &iniDefinedKeys){
    std::map<std::string, std::vector<SDL_Scancode>>::iterator it;
    for (it = iniDefinedKeys.begin(); it != iniDefinedKeys.end(); it++){
      Input::Keybinds::keys.insert({it->first, it->second});
    }
  }

}