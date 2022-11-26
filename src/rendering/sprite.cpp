#include <memory>
#include <string>
#include <algorithm>

#include "rendering.h"
using namespace SDLA;


Rendering::Sprite::Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera, std::string window)
: Renderable(info, layer, window){
  SDLSurface* mySur = loadSurface(info->fileName);
  
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

std::shared_ptr<SDLA::Rendering::Sprite> Rendering::Sprite::addImage(std::string window, int layer, Rendering::SpriteInfo* info, bool ignoreCamera){
  // workingWindow = window;



  std::shared_ptr<Sprite> s = std::make_shared<Sprite>(info, layer, ignoreCamera, window);

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  s->ownerGroup = sG;
  sG->sprites.push_back(s);
  sG->ignoreCamera = ignoreCamera;
  sG->info = std::make_shared<SpriteInfo>();
  windows[window]->getLayer(layer)->groups.push_back(sG);
    // SDLA::Rendering::TextInfo* textInfo = new SDLA::Rendering::TextInfo();
    // textInfo->info = new SDLA::Rendering::SpriteInfo();
    // textInfo->info->offset = info->offset;
    // textInfo->textureText = std::to_string(windows[window]->getWriteBuffer()[0]->id) + " " + std::to_string(windows[window]->getWriteBuffer()[1]->id) ;
    // textInfo->fontName = "assets/Minecraft.ttf";
    // textInfo->size = 12;
    // textInfo->textColor = (SDL_Color) {255,255,255};
    // SDLA::Rendering::Text::loadText(window, 1, textInfo, true);
  return s;
}

// Add ignore camera
std::vector<std::shared_ptr<SDLA::Rendering::Sprite>> Rendering::Sprite::addImageGroup(std::string window, int layer, SpriteInfo* groupInfo, std::vector<SpriteInfo*> group, bool ignoreCamera){

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->ignoreCamera = ignoreCamera;
  sG->info = std::shared_ptr<SpriteInfo>(groupInfo);
  std::vector<std::shared_ptr<Sprite>> sV;
  for(int i = 0; i < group.size(); i++){
    std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i], layer, ignoreCamera, window);
    s->ownerGroup = sG;
    sV.push_back(s);
    sG->sprites.push_back(s);
  }

  windows[window]->getLayer(layer)->groups.push_back(sG);
  return sV;
}

void Rendering::Sprite::changeSurface(std::string fileN){
  if(surfaces[info->fileName].useCount != -1){
    surfaces[info->fileName].useCount -= 1;
    if(surfaces[info->fileName].useCount == 0) surfaces.erase(info->fileName);
  }
  
  loadSurface(fileN);
  info->fileName = fileN;
  texQueued = true;
}

// TODO check if image exists first
Rendering::SDLSurface* Rendering::Sprite::loadSurface(std::string fileName, bool keepImgInMemory){
  if(!surfaces.count(fileName)){
    Rendering::SDLSurface newSur;
    newSur.sur = IMG_Load(fileName.c_str());
    newSur.fileName = fileName;
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
