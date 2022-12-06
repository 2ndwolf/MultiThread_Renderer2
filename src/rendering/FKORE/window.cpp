#include <string>
#include <memory>
#include <thread>
#include <algorithm>

#include <SDL.h>


// #include "rendering.h"
#include "identifier.h"
#include "TSRPrimitives.h"
#include "fkore.h"
#include "window.h"
#include "information.h"
#include "renderable.h"
#include "renderables.h"
#include "AT.h"

namespace FK{

    void Window::mane(){
      // int lastEventTime;
      std::shared_ptr<FK::AT::SpriteGroup> sG = std::make_shared<FK::AT::SpriteGroup>(std::make_shared<FK::AT::Information>(name));
      ORE::Renderable rend = ORE::Renderable(
        std::make_shared<FK::AT::SpriteInformation>(std::make_shared<FK::AT::Information>(name)),
        0, "Abstraction", sG
      );
      // sG->sprites.push_back(rend);
      // windows[name]->getLayer(layer)->groups.push_back(
      while(!pendingErase) {
        // if(events.lastPoll != lastEventTime){
        //   lastEventTime = events.lastPoll;
        //   pollEvents();
        // }
        display("Abstraction", rend);
      }
      windows[name] = nullptr;
    }

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
    this->hasOwnThread = hasOwnThread;

    if(windows.size() == 1) {
      currentWindow = name;
    }

    // std::shared_ptr<Window> window = std::shared_ptr<Window>(this);
    // windows.insert({name, window});
    // if(hasOwnThread){
    //   std::thread(mane, window).detach();
    // } else openMulti();

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
    windows.insert({name, std::make_shared<Window>(
     layerCount,
     windowSize,
     name,
     hasOwnThread,
     position,
     mode)});

    if(hasOwnThread){
      std::thread(mane, windows[name]).detach();
    } else openMulti();

    return name;
  }

  void Window::display(std::string window, ORE::Renderable rend){
    // std::shared_ptr<Window> window = Window::getWindow(window);
    SDL_RenderClear(context);
    // updateGroupBuffer();

    /*
    // Layers shouldn't be erased
    for(int i = 0; i < readBuffer.size(); i++){
      
      if(readBuffer[i].groups.size() > 0){

        readBuffer[i].groups.erase(
          std::remove_if(std::begin(readBuffer[i].groups), std::end(readBuffer[i].groups),
            [&] (std::shared_ptr<FK::AT::SpriteGroup> sG) -> bool {
              
              // mutex.lock();
              //   FK::AT::SpriteGroup& sG = *sprG;
              // mutex.unlock();

              if (sG->pendingErase) return true;
              std::shared_ptr<FK::AT::SuperGroup> superGroup = sG->superGroup;
              while(superGroup != nullptr){
                if(superGroup->pendingErase) return true;
                
                superGroup = superGroup->superGroup;
              }

              sG->sprites.erase(
                std::remove_if(std::begin(sG->sprites), std::end(sG->sprites),
                  [&] (std::shared_ptr<FK::ORE::Renderable> s) -> bool {
                  // mutex.lock();
                  //   FK::ORE::Renderable& s = *spr;
                  // mutex.unlock();
                  if (s->pendingErase) {
                    return true;
                  }
                  // FK::AT::SpriteInformation* sInformation;
                  SDL_Rect targetRect;

                  {
                    // FK::AT::SpriteInformation::Busy(window);
                    // s->sprInfoRead = s->sprInfoRead;
                    SDL_Rect targetRect = s->getSDLRect();
                  }


                  if(s->texQueued){
                    FK::AT::SpriteInformation::Busy(window);
                    s->setTexture(SDL_CreateTextureFromSurface(context, FKORE::surfaces[s->sprInfoWrite->fileName].sur));
                    Bounds area = s->sprInfoWrite->area;
                    if(area.box.width == 0 || area.box.height == 0){
                      SDL_QueryTexture(s->getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                      s->setCrop(area, true);
                    }
                    s->texQueued = false;
                  }

                  superGroup = sG->superGroup;
                  while(superGroup != nullptr){
                    if(superGroup->hidden) return false;
                    superGroup = superGroup->superGroup;
                  }

                  if(!(readBuffer[i].hidden | 
                  sG->hidden  |
                  s->sprInfoRead->hidden )){
                    
                    // std::shared_ptr<SpriteInformation> sGInformation = sG->information;

                    // mutex.lock();

                    if(s->getIgnoreCamera() || sG->superGroup == nullptr){
                      targetRect.x = readBuffer[i].offset.x + s->sprInfoRead->bounds.pos.x + sG->bounds.pos.x;
                      targetRect.y = readBuffer[i].offset.y + s->sprInfoRead->bounds.pos.y + sG->bounds.pos.y;
                    } else {
                      Vec2 superMov = {0,0};
                      superGroup = sG->superGroup;
                      while(superGroup != nullptr){
                        superMov = superMov + superGroup->bounds.pos + sG->worldPos;
                        superGroup = superGroup->superGroup;
                      }
                      targetRect.x += camPos.x + readBuffer[i].offset.x + s->sprInfoRead->bounds.pos.x + sG->bounds.pos.x;
                      targetRect.y += camPos.y + readBuffer[i].offset.y + s->sprInfoRead->bounds.pos.y + sG->bounds.pos.y;

                    }
                    SDL_Rect meow = {0,0,40,40};
                    // Calculer l'angle du renderable et la position par rapport aux rotations préséquentes
                    // SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &targetRect, sG->angle, (SDL_Point*) s->sprInfoRead->rotCenter, (SDL_RendererFlip) s->sprInfoRead->flip);
                    SDL_RenderCopyEx(context, s->getTexture(), s->getSRCRect(), &meow, sG->angle, (SDL_Point*) s->sprInfoRead->rotCenter, (SDL_RendererFlip) s->sprInfoRead->flip);
                    // mutex.unlock();
                  }
                  // delete sInformation;
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
    */
                  //    if(rend.texQueued){
                  //   // FK::AT::SpriteInformation::Busy(window);
                  //   rend.setTexture(SDL_CreateTextureFromSurface(context, FKORE::surfaces[rend.sprInfoWrite->fileName].sur));
                  //   Bounds area = rend.sprInfoWrite->area;
                  //   if(area.box.width == 0 || area.box.height == 0){
                  //     SDL_QueryTexture(rend.getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                  //     rend.setCrop(area, true);
                  //   }
                  //   rend.RefreshSpriteInfo();
                  //   rend.texQueued = false;
                  // }
                     if(rend.texQueued){
                    // FK::AT::SpriteInformation::Busy(window);
                    rend.setTexture(SDL_CreateTextureFromSurface(context, FKORE::surfaces["assets/gen_specialchest.png"].sur));
                    Bounds area = rend.sprInfoRead->area;
                    if(area.box.width == 0 || area.box.height == 0){
                      SDL_QueryTexture(rend.getTexture(), NULL, NULL, &area.box.width, &area.box.height);
                      rend.setCrop(area, true);
                    }
                    rend.RefreshSpriteInfo();
                    rend.texQueued = false;
                  }

    SDL_Rect* rect = rend.getSDLRect();
    SDL_RenderCopyEx(context, rend.getTexture(), rend.getSRCRect(), rect, 0, (SDL_Point*) rend.sprInfoRead->rotCenter, (SDL_RendererFlip) rend.sprInfoRead->flip);

    SDL_RenderPresent(context);
    // TODO 
    SDL_Delay(500); 
    ms = SDL_GetTicks();
    if(ms < 5000) {} else if (ms <= 10000) {fps++;}
    if(ms >= 10000 && !fpsShown){
      // AT::addSprite(name,1,std::make_shared<AT::SpriteInformation>(), true);
      AT::loadText(name, 1, 
        std::make_shared<AT::TextInformation>(
        std::make_shared<AT::Information>(Bounds{0,350,0,0}),
        std::to_string(fps/5),
        "assets/Minecraft.ttf",
        12,
        (SDL_Color) {255,255,255}
        ), true);
      fpsShown = true;
    }
  }

  void Window::pollEvents(){
    switch (events.event.window.event){
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
