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
      : writeBuffer(layerCount, std::make_shared<Layer>()){

  size = windowSize;
  // this->layerCount = layerCount;
  // this->winID = Identifier::newWinID();
  // this->focusedWin = winID;

  window = SDL_CreateWindow(name.c_str(),
                            position.x, 
                            position.y, 
                            windowSize.width, windowSize.height, mode);

  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE; 
  context = SDL_CreateRenderer(window, -1, render_flags);
  this->name = name;
  
  // windows.push_back(std::make_shared<this>);

  // groupBufferOld = new std::map<int, std::shared_ptr<SpriteGroup>>();
  // groupBuffer = new std::map<int, std::shared_ptr<SpriteGroup>>();
}

// SDL_Texture* Rendering::Window::createTexture(SDL_Surface* surface){

//   Rendering::mutex.lock();
//   // context = context2;
//   SDL_Texture* texture = SDL_CreateTextureFromSurface(context, surface);
//   Rendering::mutex.unlock();
//   return texture;
// }

void Rendering::Window::display(){
  SDL_RenderClear(context);
  Vec2 offset;

  // Check if = initialization is required.
  // Vec2 camPos = (Vec2) {0,0};

  // Layers shouldn't be erased
  for(int i = 0; i < readBuffer.size(); i++){

    // firstSpriteGroup is actually not a group, so disable group properties
    // is this still true?
    // bool firstSpriteGroup = false;

    updateGroupBuffer();
    
    if(readBuffer[i]->groups.size() != 0){
      readBuffer[i]->groups.erase(
        std::remove_if(std::begin(readBuffer[i]->groups), std::end(readBuffer[i]->groups),
          [&] (std::shared_ptr<SpriteGroup> sG) -> bool {
            if (sG->pendingErase) {
              return true;
            }

            // if(firstSpriteGroup){
            //   offset = readBuffer[i]->offset;
            // } else {
              // offset.x = readBuffer[i]->offset.x + sG->worldPos.x;
              // offset.y = readBuffer[i]->offset.y + sG->worldPos.y;
            // }



            sG->sprites.erase(
              std::remove_if(std::begin(sG->sprites), std::end(sG->sprites),
                [&] (std::shared_ptr<Renderable> s) -> bool {
                if (s->pendingErase) {
                  return true;
                }
                if(s->texQueued){
                  s->setTexture(SDL_CreateTextureFromSurface(context, Rendering::surfaces[s->getFileName()].sur));
                  Bounds area = s->info->area;
                  if(area.box.width == 0 || area.box.height == 0){
                    SDL_QueryTexture(s->getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                    s->setCrop(area, true);
                  }
                  s->texQueued = false;
                }
                if(!(readBuffer[i]->hidden || 
                sG->info->hidden  ||
                s->info->hidden )){
                  std::shared_ptr<SpriteInfo> sInfo = s->info;
                  std::shared_ptr<SpriteInfo> sGInfo = sG->info;

                  // Vec2* spriteOffset = &sInfo->offset;

                  SDL_Rect sdlRect = *s->getSDLRect();

                  // if(firstSpriteGroup){
                  //   WorldPos spriteWorldPos = sInfo->pos.worldPos;
                  //   if(sInfo->ignoreCamera){
                  //     sdlRect.x = spriteWorldPos.x + offset.x + spriteOffset->x;
                  //     sdlRect.y = spriteWorldPos.y + offset.y + spriteOffset->y;
                  //   } else {
                  //     sdlRect.x = spriteWorldPos.x - camPos.x + offset.x + spriteOffset->x;
                  //     sdlRect.y = spriteWorldPos.y - camPos.y + offset.y + spriteOffset->y;
                  //   }
                  //   SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, sInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                  // } else {
                    // TODO take into account the group's worldpos ktnxplz
                    if(s->getIgnoreCamera()){
                      sdlRect.x = readBuffer[i]->offset.x + sInfo->offset.x + sGInfo->offset.x;
                      sdlRect.y = readBuffer[i]->offset.y + sInfo->offset.y + sGInfo->offset.y;
                    } else {
                      sdlRect.x = readBuffer[i]->offset.x + sInfo->offset.x + (sGInfo->worldPos.x - camPos.x);
                      sdlRect.y = readBuffer[i]->offset.y + sInfo->offset.y + (sGInfo->worldPos.y - camPos.y);
                    }
                    SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, sGInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                  // }
                }
                return false;
              }
              ),
              std::end(sG->sprites)
            );
            // firstSpriteGroup = false;
            return false;
          }
          ),
          std::end(readBuffer[i]->groups)
      );
    }
  }
  SDL_RenderPresent(context);
  // TODO 
  SDL_Delay(1000 / 500); 

}

