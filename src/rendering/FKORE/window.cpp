#include <string>
#include <memory>
#include <thread>
#include <algorithm>

#include <SDL.h>


// #include "rendering.h"
#include "identifier.h"
#include "primitives.h"
#include "renderable.h"
#include "renderables.h"
#include "fkore.h"
#include "window.h"
#include "information.h"
#include "AT.h"

namespace FK{

  Window::Window(
          int layerCount,
          Box windowSize,
          std::string name,
          bool hasOwnThread,
          Vec2 position,
          SDL_WindowFlags mode
          ){

    writeBuffer = std::vector<Layer>(layerCount);
    // for(int i = 0; i < layerCount; i++){
    //   writeBuffer.push_backLayer());
    // }

    size = windowSize;

    // for(std::shared_ptr<Layer> lay : writeBuffer){
    //   lay->id = FK:ORE:::newID();
    // }

    window = SDL_CreateWindow(name.c_str(),
                              position.x, 
                              position.y, 
                              windowSize.width, windowSize.height, mode);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC |  SDL_RENDERER_TARGETTEXTURE; 
    context = SDL_CreateRenderer(window, -1, render_flags);
    this->name = name;

    if(windows.size() == 1) {
      currentWindow = name;
    }

    // windows.insert({name, std::shared_ptr<Window>(this)});

  }

  std::string Window::newWindow(
    int layerCount,
    Box windowSize,
    std::string name,
    bool hasOwnThread,
    Vec2 position,
    SDL_WindowFlags mode
  ){
    Window* win = new Window(
     layerCount,
     windowSize,
     name,
     hasOwnThread,
     position,
     mode
    );
    windows.insert({name, std::shared_ptr<Window>(win)});
    // delete win;
    if(hasOwnThread){
      win->hasOwnThread = true;
      win->renderThread = std::thread(mane, win);
    } else openMulti();

    return name;
  }

  void Window::display(){
    SDL_RenderClear(context);
    updateGroupBuffer();

    // Layers shouldn't be erased
    for(int i = 0; i < readBuffer.size(); i++){
      
      if(readBuffer[i].groups.size() != 0){

        readBuffer[i].groups.erase(
          std::remove_if(std::begin(readBuffer[i].groups), std::end(readBuffer[i].groups),
            [&] (std::shared_ptr<FK::AT::SpriteGroup> sG) -> bool {
              if (sG->pendingErase) return true;
              std::shared_ptr<FK::AT::SuperGroup> superGroup = sG->superGroup;
              while(superGroup != nullptr){
                if(superGroup->pendingErase) return true;
                
                superGroup = superGroup->parentGroup;
              }

              sG->sprites.erase(
                std::remove_if(std::begin(sG->sprites), std::end(sG->sprites),
                  [&] (std::shared_ptr<FK::ORE::Renderable> s) -> bool {
                  if (s->pendingErase) {
                    return true;
                  }
                  if(s->texQueued){
                    s->setTexture(SDL_CreateTextureFromSurface(context, FKORE::surfaces[s->information->fileName].sur));
                    Bounds area = s->information->area;
                    if(area.box.width == 0 || area.box.height == 0){
                      SDL_QueryTexture(s->getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                      s->setCrop(area, true);
                    }
                    s->texQueued = false;
                  }

                  superGroup = sG->superGroup;
                  while(superGroup != nullptr){
                    if(superGroup->hidden) return false;
                    superGroup = superGroup->parentGroup;
                  }

                  if(!(readBuffer[i].hidden | 
                  sG->hidden  |
                  s->information->hidden )){
                    std::shared_ptr<FK::AT::SpriteInformation> sInformation = s->information;
                    // std::shared_ptr<SpriteInformation> sGInformation = sG->information;

                    // mutex.lock();
                    SDL_Rect sdlRect = *s->getSDLRect();

                    if(s->getIgnoreCamera() || sG->superGroup == nullptr){
                      sdlRect.x = readBuffer[i].offset.x + sInformation->offset.x + sG->offset.x;
                      sdlRect.y = readBuffer[i].offset.y + sInformation->offset.y + sG->offset.y;
                    } else {
                      Vec2 superMov = {0,0};
                      superGroup = sG->superGroup;
                      while(superGroup != nullptr){
                        superMov = superMov + superGroup->offset + superGroup->worldPos;
                        superGroup = superGroup->parentGroup;
                      }
                      sdlRect.x += superMov.x - camPos.x + readBuffer[i].offset.x + sInformation->offset.x + sG->offset.x;
                      sdlRect.y += superMov.y - camPos.y + readBuffer[i].offset.y + sInformation->offset.y + sG->offset.y;

                    }
                    SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &sdlRect, s->getAngleFromGroup ? sG->angle : sInformation->angle, (SDL_Point*) sInformation->rotCenter, (SDL_RendererFlip) sInformation->flip);
                    // mutex.unlock();
                  }
                  return false;
                }
                ),
                std::end(sG->sprites)
              );
              // if(sG->superGroup!=nullptr) sG->superGroup->childFramesDrawn = true;
              return false;
            }
            ),
            std::end(readBuffer[i].groups)
        );
        }
      }
    SDL_RenderPresent(context);
    // TODO 
    // SDL_Delay(16); 
    ms = SDL_GetTicks();
    if(ms < 5000) {} else if (ms <= 10000) {fps++;}
    if(ms >= 10000 && !fpsShown){
      // AT::addSprite(name,1,std::make_shared<AT::SpriteInformation>(), true);
      AT::loadText(name, 1, 
        std::make_shared<AT::TextInformation>(
        std::make_shared<AT::Information>(Vec2{0,350}),
        std::to_string(fps/5),
        "assets/Minecraft.ttf",
        12,
        (SDL_Color) {255,255,255}
        ), true);
      fpsShown = true;
    }
  }

  void Window::pollEvents(SDL_Event event){
    switch (event.window.event){
      case SDL_WINDOWEVENT_RESIZED:
        break;
      case SDL_WINDOWEVENT_MOVED:
        // {
          // std::shared_ptr<SDLA::Rendering::Window> window = SDLA::Rendering::getCurrentWindow();
          // SDL_Window* SDLWindow = window->getSDL_Window();
        SDL_GetWindowPosition(window, &position.x, &position.y);
        // }
        break;
      case SDL_WINDOWEVENT_SHOWN:
        break;
      case SDL_WINDOWEVENT_HIDDEN:
        break;
      case SDL_WINDOWEVENT_EXPOSED:
        break;
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        break;
      case SDL_WINDOWEVENT_MINIMIZED:
        break;
      case SDL_WINDOWEVENT_MAXIMIZED:
        break;
      case SDL_WINDOWEVENT_RESTORED:
        break;
      case SDL_WINDOWEVENT_ENTER:
        break;
      case SDL_WINDOWEVENT_LEAVE:
        break;
      case SDL_WINDOWEVENT_FOCUS_GAINED:
        currentWindow = name;
        break;
      case SDL_WINDOWEVENT_FOCUS_LOST:
        break;
      case SDL_WINDOWEVENT_CLOSE:
        close();
        break;
      case SDL_WINDOWEVENT_TAKE_FOCUS:
        break;
      case SDL_WINDOWEVENT_HIT_TEST:
        break;
    }
  }

}
