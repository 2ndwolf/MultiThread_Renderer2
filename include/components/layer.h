#ifndef _MTR_LAYER_h_
#define _MTR_LAYER_h_

#include <vector>
#include <string>

#include "primitives.h"
// #include "window.h"


namespace MTR{
  namespace RND{
    struct Layer{
      // inline Layer(){};

      int layerNumber = 0;
      std::vector<std::string> windows = std::vector<std::string>();
      Vec2 offset = {0,0};
      bool hidden = false;

      // inline ~Layer(){};

      void update(Layer layer);
    };
  }
}

#endif