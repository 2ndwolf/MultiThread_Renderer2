//=================================
// include guard
#ifndef _SDLA_preferences_h_
#define _SDLA_preferences_h_

#include <string>
#include <vector>
#include <map>

#include <SDL_scancode.h>

// #include "input.h"


namespace FK {
  namespace ORE{
    struct userPreferences {
      // std::string playerImage;
      std::map<std::string, std::string> sounds, images;
      std::map<std::string, std::vector<SDL_Scancode>> keys;
    };

    class uPreferences{
      public:
      inline static userPreferences uPrefs;
    };

    void parseIni(const std::string &fileName);
  }
}

#endif