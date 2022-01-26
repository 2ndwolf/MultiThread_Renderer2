#include <string>

#include <SDL2/SDL.h> 

#include "rendering.h"
#include "identifier.h"

using namespace SDLA;
ID Identifier::currtID;

Rendering::Window::Window(
        int layerCount,
        Box windowSize,
        std::string name,
        Vec2 position,
        SDL_WindowFlags mode
        )
      : layers(layerCount, std::make_shared<Layer>()){

  size = windowSize;
  this->layerCount = layerCount;

  window = SDL_CreateWindow(name.c_str(),
                            position.x, 
                            position.y, 
                            windowSize.width, windowSize.height, mode);

  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE; 
  context = SDL_CreateRenderer(window, -1, render_flags);

  // groupBufferOld = new std::map<int, std::shared_ptr<SpriteGroup>>();
  // groupBuffer = new std::map<int, std::shared_ptr<SpriteGroup>>();
}

SDL_Texture* Rendering::Window::createTexture(SDL_Surface* surface){

  Rendering::mutex.lock();
  // context = context2;
  SDL_Texture* texture = SDL_CreateTextureFromSurface(context, surface);
  Rendering::mutex.unlock();
  return texture;
}

void Rendering::Window::display(){
  SDL_RenderClear(context);
  Vec2 offset;

  // Check if = initialization is required.
  Vec2 camPos = (Vec2) {0,0};

  // Layers shouldn't be erased
  for(int i = 0; i < layers.size(); i++){

    updateGroupBuffer(i);

    if(layers[i]->groups.size() != 0){
      std::map<SDLA::ID, std::shared_ptr<SpriteGroup>>::iterator grIt;
      for(grIt = layers[i]->groups.begin(); grIt != layers[i]->groups.end(); ){
        if(grIt->second->pendingErase) grIt = layers[i]->groups.erase(grIt);
        else {

          offset.x = layers[i]->offset.x + grIt->second->info->pos.offset.x + grIt->second->info->pos.worldPos.x;
          offset.y = layers[i]->offset.y + grIt->second->info->pos.offset.y + grIt->second->info->pos.worldPos.y;

          std::map<SDLA::ID, std::shared_ptr<Sprite>>::iterator spIt;
          for(spIt = grIt->second->sprites.begin(); spIt != grIt->second->sprites.end(); ){
            if(spIt->second->pendingErase) spIt = grIt->second->sprites.erase(spIt);
            else {

              if(spIt->second->texQueued){
                spIt->second->texture = SDL_CreateTextureFromSurface(context, Rendering::surfaces[spIt->second->fileName].sur);
                Bounds* area = &spIt->second->getInfo()->area;
                if(area->box.width == 0 || area->box.height == 0){
                  SDL_QueryTexture(spIt->second->texture, NULL, NULL, &area->box.width, &area->box.height);
                  spIt->second->setCrop((*area), true);
                }
                spIt->second->texQueued = false;
              }
              if(!(layers[i]->hidden || 
              grIt->second->info->hidden  ||
              spIt->second->getInfo()->hidden )){
                SpriteInfo* sInfo = spIt->second->getInfo();
                SpriteInfo* sGInfo = grIt->second->info;

                Vec2* spriteOffset = &sInfo->pos.offset;

                SDL_Rect sdlRect = *spIt->second->getSDLRect();

                  // TODO take into account the group's worldpos ktnxplz
                  if(sGInfo->ignoreCamera){
                    sdlRect.x = offset.x + spriteOffset->x;
                    sdlRect.y = offset.y + spriteOffset->y;
                  } else {
                    sdlRect.x = camPos.x + offset.x + spriteOffset->x;
                    sdlRect.y = camPos.y + offset.y + spriteOffset->y;
                  }
                  SDL_RenderCopyEx(context, spIt->second->getTexture(), spIt->second->getSRCRect(), &sdlRect, sGInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                }

              spIt++;
            }
          }

          grIt++;
        }
      }
    }
  }

  SDL_RenderPresent(context);
  // TODO 
  SDL_Delay(1000 / 500); 

}

SDLA::ID Rendering::Window::updImage(SDLA::idInf idInf, Rendering::SpriteInfo* info, SDLA::ID sprID){
  if(idInf.layer < 0) idInf.layer = 0;

  layers[idInf.layer]->gBufferBusy = true;

  if(sprID == SDLA::Identifier::noID()) sprID = SDLA::Identifier::newID();
  std::shared_ptr<Sprite> spr = std::make_shared<Sprite>(info);
  spr->ownerGroupID = idInf.id;
  if(layers[idInf.layer]->groupBuffer.count(idInf.id) == 0){
    std::shared_ptr<Rendering::SpriteGroup> sprG = std::make_shared<Rendering::SpriteGroup>();
    layers[idInf.layer]->groupBuffer.insert({idInf.id, sprG});
  }
  layers[idInf.layer]->groupBuffer[idInf.id]->sprites.insert({sprID, spr});

  layers[idInf.layer]->gBufferBusy = false;

  layers[idInf.layer]->groupsMirror[idInf.id]->sprites.insert({sprID, spr});
  return sprID;
}

void Rendering::Window::updGroup(SDLA::idInf idInf, Rendering::SpriteInfo* info){
  layers[idInf.layer]->groupsMirror[idInf.id]->info = info;

  layers[idInf.layer]->gBufferBusy = true;

  if(layers[idInf.layer]->groupBuffer.count(idInf.id) == 0){
    std::shared_ptr<Rendering::SpriteGroup> sprG = std::make_shared<Rendering::SpriteGroup>();
    sprG->info = info;
    sprG->updated = true;
    layers[idInf.layer]->groupBuffer.insert({idInf.id, sprG});
    // return;
  } else {
    layers[idInf.layer]->groupBuffer[idInf.id]->info = info;
    layers[idInf.layer]->groupBuffer[idInf.id]->updated = true;
  }

  layers[idInf.layer]->gBufferBusy = false;
}

SDLA::idInf Rendering::Window::newGroup(int layer){
  if(layer < 0) layer = 0;
  // std::shared_ptr<Rendering::SpriteGroup> sprG = std::make_shared<Rendering::SpriteGroup>();
  SDLA::ID groupID = SDLA::Identifier::newID();

  layers[layer]->gBufferBusy = true;

  layers[layer]->groupBuffer.insert({groupID, std::make_shared<Rendering::SpriteGroup>()});
  layers[layer]->groupsMirror.insert({groupID, std::make_shared<Rendering::SpriteGroup>()});

  layers[layer]->gBufferBusy = false;

  return {groupID, layer};
}

void Rendering::Window::updateGroupBuffer(int layer){

  while(layers[layer]->gBufferBusy){}
  layers[layer]->groupBuffer.swap(layers[layer]->groupBufferOld);

  // What if it's a modification to 
  if(layers[layer]->groupBufferOld.size() != 0){
    for (auto const& [key, val] : layers[layer]->groupBufferOld){
      SDLA::ID groupID = key;
      std::map<SDLA::ID, std::shared_ptr<SDLA::Rendering::SpriteGroup>>::iterator it = layers[layer]->groups.find(key);
      
      // insert new groups
      if(it == layers[layer]->groups.end()){
        layers[layer]->groups.insert({groupID, val});
      
      // modify old groups
      } else {
        if(val->updated){
          layers[layer]->groups[groupID]->info = val->info;
        }
        for (auto const& [key2, val2] : val->sprites){
          if(layers[layer]->groups[groupID]->sprites.count(key2) == 0){
            layers[layer]->groups[groupID]->sprites.insert({key2, val2});
          } else layers[layer]->groups[groupID]->sprites[key2] = val2;
        }
      }
    }
  }
  // Erase old buffer 
  layers[layer]->groupBufferOld.clear();
}
// std::shared_ptr<Rendering::SpriteGroup> Rendering::Window::grabGroup(SDLA::idInf groupID){
//   return layers[groupID.layer]->groupBuffer[groupID.id];
// }

// void Rendering::Window::insertToBu