//=================================
// include guard
#ifndef _input_h_
#define _input_h_

#include <map>
#include <string>
#include <vector>

#include "SDL_scancode.h"


namespace Input{
  enum KeyStates {
    UP,
    INACTIVE,
    DOWN,
    HELD
  };

  inline std::vector<std::string> scancodeStrVec((int)SDL_NUM_SCANCODES);

  void initKeyBinds(std::map<std::string, std::vector<SDL_Scancode>> &iniDefinedKeys);
  void loadScancodes();
  std::vector<SDL_Scancode> findScancode(std::vector<std::string> keycodeAsStr);

  // void updateMouse(entt::registry &registry);

  int pollEvents();
  void updateInputStates(std::map<SDL_Scancode, KeyStates> &inputStates);
  void MusicFun();

  bool keyPressed(std::string key);
  bool keyHeld(std::string key);
  bool keyUp(std::string key);
  bool keyDown(std::string key);

  class KeyStatesMap{
    public:
    inline static std::map<SDL_Scancode, KeyStates> keycodeStates;
  };

  class Keybinds{
    public:
    inline static std::map<std::string, std::vector<SDL_Scancode>> keys;
  };

}

#endif