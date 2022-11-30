#include <memory>
#include <string>

#include "rendering.h"
#include "identifier.h"
#include <SDL_ttf.h>

using namespace SDLA;

std::string Rendering::newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      Vec2 position,
      SDL_WindowFlags mode,
      bool hasOwnThread
      ){
  std::shared_ptr<Window> window = std::make_shared<Window>(layerCount, windowSize, name, position, mode, hasOwnThread);
  // if(window->)
  
  windows.insert({name, window});
  if(windows.size() == 1) {
    currentWindow = name;
    // workingWindow = name;
  }
  return name;
}

void SDLA::Rendering::setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG){

  Vec2 minOffset = {INT_MAX,INT_MAX};
  Box groupBox = {0,0};

  for(std::shared_ptr<Renderable> s : sG->sprites){
    std::shared_ptr<SpriteInfo> sInfo = s->info;
    SDL_Rect* sdlRect = s->getSDLRect();

    // Ajouter les calculs de caméra
    groupBox.width  = std::max(groupBox.width,  sdlRect->w + sInfo->offset.x);
    groupBox.height = std::max(groupBox.height, sdlRect->h + sInfo->offset.y);

    minOffset.x = std::min(minOffset.x, sInfo->offset.x);
    minOffset.y = std::min(minOffset.y, sInfo->offset.y);
  }

  Vec2 midSize = (Vec2) {minOffset.x + groupBox.width, minOffset.y + groupBox.height} / (Vec2) {2,2};

  for(std::shared_ptr<Renderable> s : sG->sprites){
    s->info->rotCenter = new Vec2();
    *s->info->rotCenter = midSize - s->info->offset;
  }

}