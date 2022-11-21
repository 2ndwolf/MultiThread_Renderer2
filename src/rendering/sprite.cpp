#include <memory>
#include <string>
#include <algorithm>

#include "rendering.h"
using namespace SDLA;

std::mutex Rendering::Sprite::mtx;

Rendering::Sprite::Sprite(Rendering::SpriteInfo* info){
  SDLSurface* mySur = Rendering::loadSurface(info->fileName);

  this->fileName = info->fileName;
  this->info = info;
  myWin = win;
  

  // texture = win->queueTexture(mySur->sur);

  srcRect.x = info->area.pos.x;
  srcRect.y = info->area.pos.y;

  if(info->area.box.width != 0 && info->area.box.height != 0){
    srcRect.w = info->area.box.width;
    srcRect.h = info->area.box.height;
    sdlRect.w = info->area.box.width;
    sdlRect.h = info->area.box.height;
    sdlRect.x = 0;
    sdlRect.y = 0;
    // SDL_QueryTexture(texture, NULL, NULL, &info.area.box.width, &info.area.box.height); 
  }// else {
  //   SDL_QueryTexture(texture, NULL, NULL, &sdlRect.w, &sdlRect.h); 
  //   srcRect.w = sdlRect.w;
  //   srcRect.h = sdlRect.h;
  //   sdlRect.x = 0;
  //   sdlRect.y = 0;
  // }
}

std::shared_ptr<Rendering::Sprite> Rendering::Sprite::addImage(std::shared_ptr<SDLA::Rendering::Window> window, int layer, Rendering::SpriteInfo* info){
  setWorkingWindow(window);

  if(layer > win->getLayerCount()) layer =  win->getLayerCount();
  else if(layer < 0) layer = 0;

  // std::vector<std::shared_ptr<SpriteGroup>>* groups = &win->getLayer(layer)->groups;
  // std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(info);
  // if(groupID < 0){
  //   groupID = 0;
  //   if(groups->size() == 0){
      
  //     groups->push_back(std::make_shared<SpriteGroup>());
  //   }
  // }
  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->sprites.push_back(std::make_shared<Sprite>(info));
  sG->info = info;
  mtx.lock();
  win->getLayer(layer)->groupBuffer.push_back(sG);
  mtx.unlock();
  return sG->sprites[0];
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

void Rendering::Sprite::setAngle(int degrees){
  if(ownerGroup == nullptr){
    mtx.lock();
    info->angle = degrees;
    mtx.unlock();
  } else {
    // mtx.lock();
    // ownerGroup->info.angle = degrees;
    // mtx.unlock();
  }
}

void Rendering::Sprite::addAngle(int degrees){
  if(ownerGroup == nullptr){
    mtx.lock();
    info->angle += degrees;
    mtx.unlock();
  } else {
    // mtx.lock();
    // ownerGroup->info.angle += degrees;
    // mtx.unlock();
  }
}

void Rendering::Sprite::setRotationCenter(Vec2 center){
  mtx.lock();
  if(info->rotCenter == nullptr) info->rotCenter = new Vec2();
  *info->rotCenter = center;
  mtx.unlock();
}

void Rendering::Sprite::setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG){
  Vec2 center = {0,0};
  SpriteInfo sInfo;
  SDL_Rect sdlRect;
  
  std::vector<Vec2> sprOffs;
  Vec2 maxLengthOffset;
  Vec2 minOffset;

  Bounds groupBounds;
  groupBounds.box = {0,0};
  groupBounds.pos = {INT_MAX,INT_MAX};
  // std::map<std::string, std::shared_ptr<Sprite>>::iterator it;
  for(std::shared_ptr<Sprite> s : sG->sprites){
    mtx.lock();
    sInfo = *s->getInfo();
    sdlRect = *s->getSDLRect();
    mtx.unlock();
    // SDL_Rect* rect = s->getSDLRect();
    
    sprOffs.push_back(sInfo.pos.offset);
    groupBounds.box.width = std::max(groupBounds.box.width, sdlRect.w + (sInfo.pos.offset.x));
    groupBounds.box.height = std::max(groupBounds.box.height, sdlRect.h + (sInfo.pos.offset.y));

    if(groupBounds.box.width == sdlRect.w + (sInfo.pos.offset.x)){
      maxLengthOffset.x = groupBounds.box.width;
    }
    if(groupBounds.box.height == sdlRect.h + (sInfo.pos.offset.y)){
      maxLengthOffset.y = groupBounds.box.height;
    }

    minOffset.x = std::min(minOffset.x, sInfo.pos.offset.x);
    minOffset.y = std::min(minOffset.y, sInfo.pos.offset.y);
  }

  Vec2 bob = (Vec2) {minOffset.x + groupBounds.box.width, minOffset.y + groupBounds.box.height};

  for(std::shared_ptr<Sprite> s : sG->sprites){
    s->setRotationCenter((Vec2){bob.x / 2  - s->getInfo()->pos.offset.x, bob.y / 2  - s->getInfo()->pos.offset.y});
  }
}

void Rendering::Sprite::setCrop(Bounds crop, bool rendererCall){
  if(!rendererCall) mtx.lock();
  info->area = crop;
  srcRect.w = crop.box.width;
  srcRect.h = crop.box.height;
  srcRect.x = crop.pos.x;
  srcRect.y = crop.pos.y;
  sdlRect.w = crop.box.width;
  sdlRect.h = crop.box.height;
  if(!rendererCall) mtx.unlock();
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