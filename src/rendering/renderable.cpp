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
  // GET WINDOW'S MUTEX
  
  if(!rendererCall) getWindow(myWindow)->mutex.lock();
  info->area = crop;
  srcRect.w = crop.box.width;
  srcRect.h = crop.box.height;
  srcRect.x = crop.pos.x;
  srcRect.y = crop.pos.y;
  sdlRect.w = crop.box.width;
  sdlRect.h = crop.box.height;
  if(!rendererCall) getWindow(myWindow)->mutex.lock();
}

SDLA::Bounds SDLA::Rendering::Renderable::getBounds(){ 
SDLA::Vec2 myOffset = windows[myWindow]->getBuffer()[layer]->offset + info->offset;
std::shared_ptr<SDLA::Rendering::SuperGroup> superGroup = ownerGroup->superGroup;
while(superGroup != nullptr) {
  myOffset = myOffset + superGroup->offset + (ownerGroup->ignoreCamera ? (Vec2){0,0} : superGroup->worldPos);
  superGroup = superGroup->parentGroup;
}
return {myOffset, {getSDLRect()->w,getSDLRect()->h}};}

