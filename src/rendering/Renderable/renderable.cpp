// #include "rendering.h"
#include <SDL.h>

#include "TSRPrimitives.h"
#include "renderable.h"
#include "window.h"
#include "information.h"
#include "fkore.h"
#include "fkenum.h"

namespace FK{
  namespace ORE{

    void Renderable::setCrop(Bounds crop, bool rendererCall){
      // GET WINDOW'S MUTEX
      // FK::AT::SpriteInformation::Busy(myWindow);
      sprInfoWrite->area = crop;
      srcRect.w = crop.box.width;
      srcRect.h = crop.box.height;
      srcRect.x = crop.pos.x;
      srcRect.y = crop.pos.y;
      tgtRect.w = crop.box.width;
      tgtRect.h = crop.box.height;
      // if(!rendererCall) Window::getWindow(myWindow)->mutex.unlock();
    }

    // This needs an update
    Bounds Renderable::getBounds(){ 
      // FK::AT::SpriteInformation* sInfo = (FK::AT::SpriteInformation*) information;
      // RefreshSpriteInfo();
      Vec2 myOffset = Window::getWindow(myWindow)->getBuffer()[layer].offset + sprInfoRead->bounds.pos;



      std::shared_ptr<AT::SuperGroup> superGroup = ownerGroup->superGroup;
      while(superGroup != nullptr) {
        myOffset = myOffset + superGroup->bounds.pos + (ownerGroup->ignoreCamera ? (Vec2){0,0} : ownerGroup->worldPos - FK::Window::getWindow(myWindow)->camPos);
        superGroup = superGroup->superGroup;
      }
      return {myOffset, {getSDLRect()->w,getSDLRect()->h}};
    }

    void Renderable::setFlip(ENUM::Flip flip){
      FK::AT::SpriteInformation::Busy();
      sprInfoWrite->flip = flip;
      
      // if(flip == ENUM::Flip::NONE) information->flip = SDL_FLIP_NONE;
      // else if(flip == ENUM::Flip::HORIZONTAL) information->flip = SDL_FLIP_HORIZONTAL;
      // else if(flip == ENUM::Flip::VERTICAL) information->flip = SDL_FLIP_VERTICAL;
    }

    void Renderable::changeSurface(std::string fileN){
      if(  FKORE::surfaces[sprInfoRead->fileName].useCount != -1){
           FKORE::surfaces[sprInfoRead->fileName].useCount -= 1;
        if(FKORE::surfaces[sprInfoRead->fileName].useCount == 0) FKORE::surfaces.erase(sprInfoRead->fileName);
      }
      
      loadSurface(fileN);
      {
        FK::AT::SpriteInformation::Busy();
        sprInfoWrite->fileName = fileN;
      }
      texQueued = true;

    }

    FKORE::SDLSurface* Renderable::loadSurface(std::string fileName, bool keepImgInMemory){
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

    // void Renderable::setRotationCenter(Vec2 center){
    //   *information->rotCenter = center;
    //         // *s->getInformation()->rotCenter = midSize - s->getInformation()->offset;

    // }
  }
}

