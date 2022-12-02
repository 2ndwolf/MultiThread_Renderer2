#ifndef _FK_enum_h_
#define _FK_enum_h_

#include <SDL.h>

namespace FK{
  namespace ENUM{
    enum KeyStates {
      UP,
      INACTIVE,
      DOWN,
      HELD
    };
    enum MouseMode{
      DEFAULT
    };

    enum Flip{
      NONE = SDL_FLIP_NONE,
      HORIZONTAL = SDL_FLIP_HORIZONTAL,
      VERTICAL = SDL_FLIP_VERTICAL
    };
  }

}

#endif