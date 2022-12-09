#ifndef _MTR_ENUMs_h_
#define _MTR_ENUMs_h_

#include <SDL.h>

namespace MTR{
  namespace ENUM{
    enum Flip{
      NONE = SDL_FLIP_NONE,
      HORIZONTAL = SDL_FLIP_HORIZONTAL,
      VERTICAL = SDL_FLIP_VERTICAL
    };
  }
}

#endif