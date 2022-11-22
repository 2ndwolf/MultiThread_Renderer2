#include <memory>
#include <string>
#include <algorithm>

#include "rendering.h"
using namespace SDLA;


Rendering::Sprite::Sprite(Rendering::SpriteInfo* info){
  SDLSurface* mySur = Rendering::loadSurface(info->fileName);

  this->fileName = info->fileName;
  this->info = info;
  myWin = win;
  
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

std::shared_ptr<Rendering::SpriteGroup> Rendering::Sprite::addImage(std::shared_ptr<SDLA::Rendering::Window> window, int layer, Rendering::SpriteInfo* info){
  setWorkingWindow(window);

  if(layer > win->getLayerCount()) layer =  win->getLayerCount();
  else if(layer < 0) layer = 0;

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->sprites.push_back(std::make_shared<Sprite>(info));
  sG->info = info;
  mtx.lock();
  win->getLayer(layer)->groupBuffer.push_back(sG);
  mtx.unlock();
  return sG;
}

// No star bCuz shared pointer, lulz
// Add an overload that will accept a groupInfo spriteInfo instaed of a groupID and will append
std::shared_ptr<Rendering::SpriteGroup> Rendering::Sprite::addImageGroup(std::shared_ptr<SDLA::Rendering::Window> window, int layer, SpriteInfo* groupInfo, std::vector<SpriteInfo*> group){
  setWorkingWindow(window);
  if(layer > win->getLayerCount()) layer =  win->getLayerCount();
  else if(layer < 0) layer = 0;

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->info = groupInfo;
  for(int i = 0; i < group.size(); i++){
    sG->sprites.push_back(std::make_shared<Sprite>(group[i]));
  }

  mtx.lock();
  win->getLayer(layer)->groupBuffer.push_back(sG);
  mtx.unlock();
  return sG;
}

void Rendering::Sprite::setPosition(WorldPos position){
  mtx.lock();
  info->pos.worldPos = position;
  mtx.unlock();
}

void Rendering::Sprite::moveFromPosition(Vec2 move){
  mtx.lock();
  info->pos.worldPos.x += move.x;
  info->pos.worldPos.y += move.y;
  mtx.unlock();
}

void Rendering::Sprite::changeSurface(std::string fileN){
  if(surfaces[fileName].useCount != -1){
    surfaces[fileName].useCount -= 1;
    if(surfaces[fileName].useCount == 0) surfaces.erase(fileName);
  }
  
  fileName = fileN;
  Rendering::loadSurface(fileName)->sur;

  texQueued = true;
}