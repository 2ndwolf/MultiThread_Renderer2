#include <memory>
#include <string>

#include "rendering.h"
#include "identifier.h"
#include <SDL_ttf.h>

using namespace SDLA;

std::shared_ptr<Rendering::Window> Rendering::newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      Vec2 position,
      SDL_WindowFlags mode
      ){
  std::shared_ptr<Window> window = std::make_shared<Window>(layerCount, windowSize, /* this->threadID,  */name, position, mode);
  // window->name = name;

  windows.insert({name, window});
  if(windows.size() == 1) {
    currentWindow = window;
    workingWindow = name;
  }
  return window;
}

void SDLA::Rendering::setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG){
  // Vec2 center = {0,0};
  // std::shared_ptr<SpriteInfo> sInfo;
  // SDL_Rect sdlRect;
  
  // std::vector<Vec2> sprOffs;
  // Vec2 maxLengthOffset;
  Vec2 minOffset;

  Box groupBounds = {0,0};
  // groupBounds.box = {0,0};
  // groupBounds.pos = {INT_MAX,INT_MAX};
  // std::map<std::string, std::shared_ptr<Sprite>>::iterator it;

  // if(sG->ignoreCamera){
  for(std::shared_ptr<Renderable> s : sG->sprites){
    std::shared_ptr<SpriteInfo> sInfo = s->info;
    SDL_Rect* sdlRect = s->getSDLRect();

    // Ajouter les calculs de caméra
    groupBounds.width  = std::max(groupBounds.width,  sdlRect->w + sInfo->offset.x);
    groupBounds.height = std::max(groupBounds.height, sdlRect->h + sInfo->offset.y);

    // if(groupBounds.box.width == sdlRect.w + (sInfo->offset.x)){
    //   maxLengthOffset.x = groupBounds.box.width;
    // }
    // if(groupBounds.box.height == sdlRect.h + (sInfo->offset.y)){
    //   maxLengthOffset.y = groupBounds.box.height;
    // }

    minOffset.x = std::min(minOffset.x, sInfo->offset.x);
    minOffset.y = std::min(minOffset.y, sInfo->offset.y);
  }

  Vec2 fullSize = (Vec2) {minOffset.x + groupBounds.width, minOffset.y + groupBounds.height};

  for(std::shared_ptr<Renderable> s : sG->sprites){
    s->info->rotCenter = &((fullSize / (Vec2) {2,2}) - s->info->offset);
  }
  // }

}


// // TODO check if image exists first
// Rendering::SDLSurface* Rendering::loadSurface(std::string fileName, bool keepImgInMemory){
//   if(!surfaces.count(fileName)){
//     Rendering::SDLSurface newSur;
//     newSur.sur = IMG_Load(fileName.c_str());
//     newSur.fileName = fileName;
//     if(keepImgInMemory) newSur.useCount = -1;
//     else newSur.useCount = 1;
//     surfaces.insert({fileName, newSur});

//   } else if (keepImgInMemory){
//     surfaces[fileName].useCount = -1;

//   } else if (surfaces[fileName].useCount != -1){
//     surfaces[fileName].useCount += 1;
//   }

//   return &surfaces[fileName];
// }

// Rendering::SDLFont* Rendering::loadFont(std::string fileName, int size){
//   std::string fontName = fileName + std::to_string(size);
//   if(!fonts.count(fontName)){
//     Rendering::SDLFont newFont;
//     newFont.font = TTF_OpenFont(fileName.c_str(), size);
//     newFont.fileName = fontName;
//     fonts.insert({fontName, newFont});
//   }

//   return &fonts[fontName];
// }


