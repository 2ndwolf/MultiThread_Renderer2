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
      FKORE::SDLSurface* mySur = loadSurface(info->fileName);
      
      srcRect.x = info->area.pos.x;
      srcRect.y = info->area.pos.y;

      if(info->area.box.width != 0 && info->area.box.height != 0){
        srcRect.w = info->area.box.width;
        srcRect.h = info->area.box.height;
        sdlRect.w = info->area.box.width;
        sdlRect.h = info->area.box.height;
        sdlRect.x = 0;
        sdlRect.y = 0;
      }
    }


  }
}
