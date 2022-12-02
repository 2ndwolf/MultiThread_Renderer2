//=================================
// include guard
#ifndef _input_h_
#define _input_h_

#include <map>
#include <string>
#include <vector>

#include "SDL_scancode.h"

#include "primitives.h"
#include "fkenum.h"


namespace FK{
  // class Mouse;

  namespace AT{
    // bool keyPressed(std::string key);
    // bool keyHeld(std::string key);
    // bool keyUp(std::string key);
    // bool keyDown(std::string key);
  }

  namespace ORE{

    inline std::vector<std::string> scancodeStrVec((int)SDL_NUM_SCANCODES);

    void initKeyBinds(std::map<std::string, std::vector<SDL_Scancode>> &iniDefinedKeys);
    void loadScancodes();
    std::vector<SDL_Scancode> findScancode(std::vector<std::string> keycodeAsStr);

    // void updateMouse(entt::registry &registry);

    int pollEvents();
    void updateInputStates(std::map<SDL_Scancode, ENUM::KeyStates> &inputStates);
    // void MusicFun();


    class KeyStatesMap{
      public:
      inline static std::map<SDL_Scancode, ENUM::KeyStates> keycodeStates;
    };

    class Keybinds{
      public:
      inline static std::map<std::string, std::vector<SDL_Scancode>> keys;
    };


  }



  class Mouse{
    public:
    // Mouse(){};
    inline static Vec2 position;
    inline static std::map<int, ENUM::KeyStates> clicks;
    inline static ENUM::MouseMode mouseMode = ENUM::MouseMode::DEFAULT;
  };

}

#endif