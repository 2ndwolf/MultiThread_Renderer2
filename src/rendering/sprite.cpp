#include <memory>
#include <string>
#include <algorithm>

#include "rendering.h"
using namespace SDLA;


// Rendering::Sprite::Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup)
// : Renderable(info, layer, window, ownerGroup){
//   SDLSurface* mySur = loadSurface(info->fileName);
  
//   srcRect.x = info->area.pos.x;
//   srcRect.y = info->area.pos.y;

//   if(info->area.box.width != 0 && info->area.box.height != 0){
//     srcRect.w = info->area.box.width;
//     srcRect.h = info->area.box.height;
//     sdlRect.w = info->area.box.width;
//     sdlRect.h = info->area.box.height;
//     sdlRect.x = 0;
//     sdlRect.y = 0;
//   }
// }
Rendering::Sprite::Sprite(std::shared_ptr<Rendering::SpriteInfo> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup)
: Renderable(info, layer, window, ownerGroup){
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

std::shared_ptr<SDLA::Rendering::Sprite> Rendering::Sprite::addImage(std::string window, int layer, std::shared_ptr<Rendering::SpriteInfo> info, bool ignoreCamera){
  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  std::shared_ptr<Sprite> s = std::make_shared<Sprite>(info, layer, ignoreCamera, window, sG);
  // s->ownerGroup = sG;
  sG->sprites.push_back(s);
  sG->ignoreCamera = ignoreCamera;
  // sG->info = std::make_shared<SpriteInfo>();
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

// DO NOT Add an overload for std::shared_ptr<SpriteGroup> where it adds the images to the group instead of creating a new group
// It won't be possible to add sprites to a spriteGroup -> segmentation fault possibility
// std::vector<std::shared_ptr<SDLA::Rendering::Sprite>> Rendering::Sprite::addImageGroup(std::string window, int layer, SpriteGroup* groupInfo, std::vector<SpriteInfo*> group){

//   std::shared_ptr<SpriteGroup> sG = std::shared_ptr<SpriteGroup>(groupInfo);
//   sG->sprites = {};
//   std::vector<std::shared_ptr<Sprite>> sV;
//   for(int i = 0; i < group.size(); i++){
//     std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i], layer, sG->ignoreCamera, window, sG);
//     sV.push_back(s);
//     sG->sprites.push_back(s);
//   }

//   windows[window]->getLayer(layer)->groups.push_back(sG);
//   return sV;
// }

// DO NOT Add an overload for std::shared_ptr<SpriteGroup> where it adds the images to the group instead of creating a new group
// It won't be possible to add sprites to a spriteGroup -> segmentation fault possibility
std::vector<std::shared_ptr<SDLA::Rendering::Sprite>> Rendering::Sprite::addImageGroup(std::string window, int layer, Info* groupInfo, std::vector<std::shared_ptr<SpriteInfo>> group){

  std::shared_ptr<SpriteGroup> sG = std::shared_ptr<SpriteGroup>(Rendering::SpriteGroup().createSpriteGroup(groupInfo));
  // delete groupInfo;
  sG->sprites = {};
  std::vector<std::shared_ptr<Sprite>> sV;
  for(int i = 0; i < group.size(); i++){
    std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i], layer, sG->ignoreCamera, window, sG);
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
