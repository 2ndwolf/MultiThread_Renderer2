#include <string>

#include "input.h"


namespace FK {
  namespace AT{
    bool keyPressed(std::string key){
      for(int i = 0; i < ORE::Keybinds::keys[key].size(); i++){
        if(ORE::KeyStatesMap::keycodeStates[ORE::Keybinds::keys[key][i]] > ENUM::KeyStates::INACTIVE) return true;
      }
      return false;
    }

    bool keyHeld(std::string key){
      for(int i = 0; i < ORE::Keybinds::keys[key].size(); i++){
        if(ORE::KeyStatesMap::keycodeStates[ORE::Keybinds::keys[key][i]] == ENUM::KeyStates::HELD) return true;
      }
      return false;
    }

    bool keyUp(std::string key){
      for(int i = 0; i < ORE::Keybinds::keys[key].size(); i++){
        if(ORE::KeyStatesMap::keycodeStates[ORE::Keybinds::keys[key][i]] == ENUM::KeyStates::UP) return true;
      }
      return false;
    }

    bool keyDown(std::string key){
      for(int i = 0; i < ORE::Keybinds::keys[key].size(); i++){
        if(ORE::KeyStatesMap::keycodeStates[ORE::Keybinds::keys[key][i]] == ENUM::KeyStates::DOWN) return true;
      }
      return false;
    }
  }

}
