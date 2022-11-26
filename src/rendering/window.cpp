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
        ){

  for(int i = 0; i < layerCount; i++){
    writeBuffer.push_back(std::make_shared<Layer>());
  }

  size = windowSize;

  for(std::shared_ptr<Layer> lay : writeBuffer){
    lay->id = SDLA::Identifier::newID();
  }

  window = SDL_CreateWindow(name.c_str(),
                            position.x, 
                            position.y, 
                            windowSize.width, windowSize.height, mode);

  Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE; 
  context = SDL_CreateRenderer(window, -1, render_flags);
  this->name = name;
  
}

void Rendering::Window::display(){
  SDL_RenderClear(context);
  updateGroupBuffer();

  // Layers shouldn't be erased
  for(int i = 0; i < readBuffer.size(); i++){
    
    if(readBuffer[i]->groups.size() != 0){
      readBuffer[i]->groups.erase(
        std::remove_if(std::begin(readBuffer[i]->groups), std::end(readBuffer[i]->groups),
          [&] (std::shared_ptr<SpriteGroup> sG) -> bool {
            if (sG->pendingErase) {
              return true;
            }

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

                  SDL_Rect sdlRect = *s->getSDLRect();

                  if(s->getIgnoreCamera()){
                    sdlRect.x = readBuffer[i]->offset.x + sInfo->offset.x + sGInfo->offset.x;
                    sdlRect.y = readBuffer[i]->offset.y + sInfo->offset.y + sGInfo->offset.y;
                  } else {
                    sdlRect.x = readBuffer[i]->offset.x + sInfo->offset.x + (sGInfo->worldPos.x - camPos.x);
                    sdlRect.y = readBuffer[i]->offset.y + sInfo->offset.y + (sGInfo->worldPos.y - camPos.y);
                  }
                  SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, sGInfo->angle, (SDL_Point*) sInfo->rotCenter, sInfo->flip);
                }
                return false;
              }
              ),
              std::end(sG->sprites)
            );
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

