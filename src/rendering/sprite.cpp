#include <memory>
#include <string>
#include <algorithm>

#include "rendering.h"
using namespace SDLA;


Rendering::Sprite::Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera)
: Renderable(info, layer){
  SDLSurface* mySur = loadSurface(info->fileName);

  // this->fileName = info->fileName;
  // this->info = info;
  windowOwnerName = workingWindow;
  
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
  workingWindow = window;

  if(layer > windows[workingWindow]->getLayerCount()) layer =  windows[workingWindow]->getLayerCount();
  else if(layer < 0) layer = 0;

  std::shared_ptr<Sprite> s = std::make_shared<Sprite>(info, layer, ignoreCamera);

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->sprites.push_back(s);
  sG->ignoreCamera = ignoreCamera;
  sG->info = std::make_shared<SpriteInfo>(info);
  // mtx.lock();
  windows[workingWindow]->getLayer(layer)->groups.push_back(sG);
  // mtx.unlock();
  return s;

  // std::shared_ptr<std::vector<std::shared_ptr<Sprite>>> fds = std::make_shared<std::vector<std::shared_ptr<Sprite>>>()
}

// No star bCuz shared pointer, lulz
// Add an overload that will accept a groupInfo spriteInfo instaed of a groupID and will append
std::vector<std::shared_ptr<SDLA::Rendering::Sprite>> Rendering::Sprite::addImageGroup(std::string window, int layer, SpriteInfo* groupInfo, std::vector<SpriteInfo*> group){
  workingWindow = window;

  if(layer > windows[workingWindow]->getLayerCount()) layer =  windows[workingWindow]->getLayerCount();
  else if(layer < 0) layer = 0;

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->info = std::make_shared<SpriteInfo>(groupInfo);
  std::vector<std::shared_ptr<Sprite>> sV;
  for(int i = 0; i < group.size(); i++){
    std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i]);
    sV.push_back(s);
    sG->sprites.push_back(s);
  }

  // mtx.lock();
  windows[workingWindow]->getLayer(layer)->groups.push_back(sG);
  // mtx.unlock();
  return sV;
}

// void Rendering::Sprite::setPosition(WorldPos position){
//   mtx.lock();
//   info->pos.worldPos = position;
//   mtx.unlock();
// }

// void Rendering::Sprite::moveFromPosition(Vec2 move){
//   mtx.lock();
//   info->pos.worldPos.x += move.x;
//   info->pos.worldPos.y += move.y;
//   mtx.unlock();
// }

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
