// #include "rendering.h"
#include <SDL.h>

#include "primitives.h"
#include "renderable.h"
#include "window.h"
#include "information.h"
#include "fkore.h"
#include "fkenum.h"

namespace FK{
  namespace ORE{

    void Renderable::setCrop(Bounds crop, bool rendererCall){
      // GET WINDOW'S MUTEX
      
      if(!rendererCall) Window::getWindow(myWindow)->mutex.lock();
      information->area = crop;
      srcRect.w = crop.box.width;
      srcRect.h = crop.box.height;
      srcRect.x = crop.pos.x;
      srcRect.y = crop.pos.y;
      sdlRect.w = crop.box.width;
      sdlRect.h = crop.box.height;
      if(!rendererCall) Window::getWindow(myWindow)->mutex.unlock();
    }

    // This needs an update
    Bounds Renderable::getBounds(){ 
      Vec2 myOffset = Window::getWindow(myWindow)->getBuffer()[layer].offset + information->offset;
      std::shared_ptr<AT::SuperGroup> superGroup = ownerGroup->superGroup;
      while(superGroup != nullptr) {
        myOffset = myOffset + superGroup->offset + (ownerGroup->ignoreCamera ? (Vec2){0,0} : superGroup->worldPos);
        superGroup = superGroup->parentGroup;
      }
      return {myOffset, {getSDLRect()->w,getSDLRect()->h}};
    }

    void Renderable::setFlip(ENUM::Flip flip){
      information->flip = flip;
      // if(flip == ENUM::Flip::NONE) information->flip = SDL_FLIP_NONE;
      // else if(flip == ENUM::Flip::HORIZONTAL) information->flip = SDL_FLIP_HORIZONTAL;
      // else if(flip == ENUM::Flip::VERTICAL) information->flip = SDL_FLIP_VERTICAL;
    }
  }
}

