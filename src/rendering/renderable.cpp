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
