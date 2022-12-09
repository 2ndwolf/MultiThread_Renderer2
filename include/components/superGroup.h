#ifndef _MTR_SUPERGROUP_h_
#define _MTR_SUPERGROUP_h_

#include <vector>

#include "renderable.h"
// #include "info.h"
#include "primitives.h"
#include "window.h"

namespace MTR{

  namespace RND{

    class SuperGroup : public Renderable {
      public:
      inline SuperGroup(){};

      static void update(SuperGroup* group);

      // bool ignoreCamera = true ;
      // Vec2 worldPos     = {0,0};

      // inline static void deepCopy(SuperGroup* source, SuperGroup* target){
      //   if(source->toUpdate){
      //     // Set from information
      //     Renderable::deepCopy(source, target);

      //     // Set from self
      //     target->worldPos     = source->worldPos     ;
      //     target->ignoreCamera = source->ignoreCamera ;
      //   }
      // }

      void refreshBounds(Renderable* rend){
        // Vec2 minOffset = {INT_MAX,INT_MAX};
        // Box groupBox = {0,0};
        bounds.box.width  = 
        std::max(rend->bounds.box.width , bounds.box.width  + bounds.pos.x);
        bounds.box.height = 
        std::max(rend->bounds.box.height, bounds.box.height + bounds.pos.y);

        Vec2 oldPos = bounds.pos;
        // TODO :: WorldPos?
        bounds.pos.x = 
        std::min(rend->bounds.pos.x, bounds.pos.x);
        bounds.pos.y = 
        std::min(rend->bounds.pos.y, bounds.pos.y);
      }
    };


  }
}

#endif