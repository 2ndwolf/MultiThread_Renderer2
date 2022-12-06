#include <memory>
#include <string>
#include <algorithm>

#include "information.h"
#include "renderable.h"
#include "renderables.h"
#include "fkore.h"

// #include "rendering.h"
// using namespace SDLA;

namespace FK{
  namespace AT{

    Sprite::Sprite(std::shared_ptr<SpriteInformation> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup)
    : ORE::Renderable(info, layer, window, ownerGroup){
      
      ORE::Renderable::loadSurface(info->fileName);
      
      // information->boxPtr = {&tgtRect.w, &tgtRect.h};

      srcRect.x = info->area.pos.x;
      srcRect.y = info->area.pos.y;

      if(info->area.box.width != 0 && info->area.box.height != 0){
        srcRect.w = info->area.box.width;
        srcRect.h = info->area.box.height;
        tgtRect.w = info->area.box.width;
        tgtRect.h = info->area.box.height;
        tgtRect.x = 0;
        tgtRect.y = 0;
      }
    }


  }
}
