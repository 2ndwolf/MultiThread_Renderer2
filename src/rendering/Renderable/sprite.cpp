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


    void Sprite::changeSurface(std::string fileN){
      if(FKORE::surfaces[information->fileName].useCount != -1){
        FKORE::surfaces[information->fileName].useCount -= 1;
        if(FKORE::surfaces[information->fileName].useCount == 0) FKORE::surfaces.erase(information->fileName);
      }
      
      loadSurface(fileN);
      information->fileName = fileN;
      texQueued = true;
    }

    // TODO check if image exists first
    FKORE::SDLSurface* Sprite::loadSurface(std::string fileName, bool keepImgInMemory){
      if(!FKORE::surfaces.count(fileName)){
        FKORE::SDLSurface newSur;
        newSur.sur = IMG_Load(fileName.c_str());
        if(newSur.sur == NULL) newSur.sur = IMG_Load(placeHolder.c_str());
        newSur.fileName = fileName;
        if(keepImgInMemory) newSur.useCount = -1;
        else newSur.useCount = 1;
        FKORE::surfaces.insert({fileName, newSur});

      } else if (keepImgInMemory){
        FKORE::surfaces[fileName].useCount = -1;

      } else if (FKORE::surfaces[fileName].useCount != -1){
        FKORE::surfaces[fileName].useCount += 1;
      }

      return &FKORE::surfaces[fileName];
    }
  }
}
