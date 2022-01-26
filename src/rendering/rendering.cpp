#include <memory>
#include <string>

#include "rendering.h"
using namespace SDLA;

std::map<std::string, Rendering::SDLSurface> Rendering::surfaces;


std::shared_ptr<Rendering::Window> Rendering::newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      Vec2 position,
      SDL_WindowFlags mode
      ){
  std::shared_ptr<Window> window = std::make_shared<Window>(layerCount, windowSize, /* this->threadID,  */name, position, mode);

  // SDLA::Rendering::Sprite::setWorkingWindow(window);
  // window->getLayer(0)->groups.push_back(std::make_shared<SDLA::Rendering::SpriteGroup>());
  // SDLA::Rendering::SpriteInfo* bodyInfo = new SDLA::Rendering::SpriteInfo();
  // bodyInfo->fileName = "assets/head440.png";

  // window->getLayer(0)->groups[0]->sprites.push_back(std::make_shared<SDLA::Rendering::Sprite>(bodyInfo));

  windows.members.push_back(window);
  return window;
}


Rendering::SDLSurface* Rendering::loadSurface(std::string fileName, bool keepImgInMemory){
  if(!surfaces.count(fileName)){
    Rendering::SDLSurface newSur;
    newSur.sur = IMG_Load(fileName.c_str());
    if(keepImgInMemory) newSur.useCount = -1;
    else newSur.useCount = 1;
    surfaces.insert({fileName, newSur});

  } else if (keepImgInMemory){
    surfaces[fileName].useCount = -1;

  } else if (surfaces[fileName].useCount != -1){
    surfaces[fileName].useCount += 1;
  }

  return &surfaces[fileName];
}

