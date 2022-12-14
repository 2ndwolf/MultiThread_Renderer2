#ifndef _MTR_SUPERGROUP_h_
#define _MTR_SUPERGROUP_h_

#include <vector>

#include "renderable.h"
// #include "info.h"
#include "primitives.h"
// #include "window.h"

namespace MTR{

  namespace RND{

    class SuperGroup : public Renderable {
      public:
      SuperGroup(std::vector<std::string> pwindows):
      Renderable(pwindows)
      {};

      static void update(SuperGroup* group);

      static void deepCopy(SuperGroup* source, SuperGroup* target);
      bool checkSanity();

      void refreshBounds(Renderable* rend);
    };


  }
}

#endif