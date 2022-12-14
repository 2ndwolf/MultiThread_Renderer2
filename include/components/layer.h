#ifndef _MTR_LAYER_h_
#define _MTR_LAYER_h_

#include <vector>
#include <string>

#include "primitives.h"
// #include "window.h"
#include "renderable.h"

namespace MTR{
  namespace RND{

    // TODO :: Should this be a renderable?
    struct Layer/* : public Renderable */{
      // inline Layer(){};

      int layerNumber = 0;
      std::vector<std::string> windows;
      Vec2 offset = {0,0};
      bool hidden = false;

      // inline ~Layer(){};

      void update(Layer layer);
    };
  }
}

#endif