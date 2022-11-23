#include <string>

#include <SDL.h> 


#include "rendering.h"
#include "identifier.h"
using namespace SDLA;

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
  this->winID = Identifier::newWinID();
  this->focusedWin = winID;

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

    // firstSpriteGroup is actually not a group, so disable group properties
    // is this still true?
    bool firstSpriteGroup = false;

    updateGroupBuffer(i);
    
    if(layers[i]->groups.size() != 0){
      layers[i]->groups.erase(
        std::remove_if(std::begin(layers[i]->groups), std::end(layers[i]->groups),
          [&] (std::shared_ptr<SpriteGroup> sG) -> bool {
            if (sG->pendingErase && !firstSpriteGroup) {
              return true;
            }

            if(firstSpriteGroup){
              offset = layers[i]->offset;
            } else {
              offset.x = layers[i]->offset.x + sG->info->pos.offset.x + sG->info->pos.worldPos.x;
              offset.y = layers[i]->offset.y + sG->info->pos.offset.y + sG->info->pos.worldPos.y;
            }



            sG->sprites.erase(
              std::remove_if(std::begin(sG->sprites), std::end(sG->sprites),
                [&] (std::shared_ptr<Renderable> s) -> bool {
                if (s->pendingErase) {
                  return true;
                }
                if(s->texQueued){
                  s->setTexture(SDL_CreateTextureFromSurface(context, Rendering::surfaces[s->getInfo()->fileName].sur));
                  Bounds area = s->getInfo()->area;
                  if(area.box.width == 0 || area.box.height == 0){
                    SDL_QueryTexture(s->getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                    s->setCrop(area, true);
                  }
                  s->texQueued = false;
                }
                if(!(layers[i]->hidden || 
                sG->info->hidden  ||
                s->getInfo()->hidden )){
                  SpriteInfo* sInfo = s->getInfo();
                  SpriteInfo* sGInfo = sG->info;

                  Vec2* spriteOffset = &sInfo->pos.offset;

                  SDL_Rect sdlRect = *s->getSDLRect();

                  if(firstSpriteGroup){
                    WorldPos spriteWorldPos = sInfo->pos.worldPos;
                    if(sInfo->ignoreCamera){
                      sdlRect.x = spriteWorldPos.x + offset.x + spriteOffset->x;
                      sdlRect.y = spriteWorldPos.y + offset.y + spriteOffset->y;
                    } else {
                      sdlRect.x = spriteWorldPos.x - camPos.x + offset.x + spriteOffset->x;
                      sdlRect.y = spriteWorldPos.y - camPos.y + offset.y + spriteOffset->y;
                    }
                    SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, sInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                  } else {
                    // TODO take into account the group's worldpos ktnxplz
                    if(sGInfo->ignoreCamera){
                      sdlRect.x = offset.x + spriteOffset->x;
                      sdlRect.y = offset.y + spriteOffset->y;
                    } else {
                      sdlRect.x = camPos.x + offset.x + spriteOffset->x;
                      sdlRect.y = camPos.y + offset.y + spriteOffset->y;
                    }
                    SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, sGInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                  }
                }
                return false;
              }
              ),
              std::end(sG->sprites)
            );
            firstSpriteGroup = false;
            return false;
          }
          ),
          std::end(layers[i]->groups)
      );
    }
  }
  SDL_RenderPresent(context);
  // TODO 
  SDL_Delay(1000 / 500); 

}

