#include "rendering.h"
#include "primitives.h"

// void SDLA::Rendering::Renderable::setAngle(int degrees){
//   if(ownerGroup == nullptr){
//     mtx.lock();
//     info->angle = degrees;
//     mtx.unlock();
//   } else {
//     // mtx.lock();
//     // ownerGroup->info.angle = degrees;
//     // mtx.unlock();
//   }
// }

// void SDLA::Rendering::Renderable::addAngle(int degrees){
//   // if(ownerGroup == nullptr){
//     mtx.lock();
//     info->angle += degrees;
//     mtx.unlock();
//   // } else {
//     // mtx.lock();
//     // ownerGroup->info.angle += degrees;
//     // mtx.unlock();
//   }
// }

// void SDLA::Rendering::Renderable::setRotationCenter(Vec2 center){
//   mtx.lock();
//   if(info->rotCenter == nullptr) info->rotCenter = new Vec2();
//   *info->rotCenter = center;
//   mtx.unlock();
// }

// void SDLA::Rendering::setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG){
//   Vec2 center = {0,0};
//   std::shared_ptr<SpriteInfo> sInfo;
//   SDL_Rect sdlRect;
  
//   std::vector<Vec2> sprOffs;
//   Vec2 maxLengthOffset;
//   Vec2 minOffset;

//   Bounds groupBounds;
//   groupBounds.box = {0,0};
//   groupBounds.pos = {INT_MAX,INT_MAX};
//   // std::map<std::string, std::shared_ptr<Sprite>>::iterator it;
//   for(std::shared_ptr<Renderable> s : sG->sprites){
//     // mtx.lock();
//     sInfo = s->info;
//     sdlRect = *s->getSDLRect();
//     // mtx.unlock();
//     // SDL_Rect* rect = s->getSDLRect();
    
//     sprOffs.push_back(sInfo.pos.offset);
//     groupBounds.box.width = std::max(groupBounds.box.width, sdlRect.w + (sInfo.pos.offset.x));
//     groupBounds.box.height = std::max(groupBounds.box.height, sdlRect.h + (sInfo.pos.offset.y));

//     if(groupBounds.box.width == sdlRect.w + (sInfo.pos.offset.x)){
//       maxLengthOffset.x = groupBounds.box.width;
//     }
//     if(groupBounds.box.height == sdlRect.h + (sInfo.pos.offset.y)){
//       maxLengthOffset.y = groupBounds.box.height;
//     }

//     minOffset.x = std::min(minOffset.x, sInfo.pos.offset.x);
//     minOffset.y = std::min(minOffset.y, sInfo.pos.offset.y);
//   }

//   Vec2 bob = (Vec2) {minOffset.x + groupBounds.box.width, minOffset.y + groupBounds.box.height};

//   for(std::shared_ptr<Renderable> s : sG->sprites){
//     s->setRotationCenter((Vec2){bob.x / 2  - s->getInfo()->pos.offset.x, bob.y / 2  - s->getInfo()->pos.offset.y});
//   }
// }

void SDLA::Rendering::Renderable::setCrop(Bounds crop, bool rendererCall){
  if(!rendererCall) mutex.lock();
  info->area = crop;
  srcRect.w = crop.box.width;
  srcRect.h = crop.box.height;
  srcRect.x = crop.pos.x;
  srcRect.y = crop.pos.y;
  sdlRect.w = crop.box.width;
  sdlRect.h = crop.box.height;
  if(!rendererCall) mutex.unlock();
}
