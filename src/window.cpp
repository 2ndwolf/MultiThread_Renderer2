#include <thread>
#include <algorithm>
#include <map>
#include <string>
#include <atomic>
#include <utility>

#include <SDL.h>

#include "buffer.h"
#include "primitives.h" 
#include "window.h"
#include "layer.h"
#include "superGroup.h"
#include "spriteGroup.h"
#include "image.h"

#include "INST.h"

namespace MTR{

  Window::Window(
          int layerCount,
          Box windowSize,
          const std::string& name,
          Vec2 position,
          bool hasOwnThread,
          SDL_WindowFlags mode
          ){

    buffer = Buffer(layerCount);

    for(int i = 0; i < layerCount; i++){
      layers.push_back(new MTR::RND::Layer(false));
      // Layer tmp();

      buffer.writeBuffer.dfrLayer.emplace((void*)&(layers[i]), MTR::RND::Layer(false));
    }

    bounds = {position, windowSize};
    this->hasOwnThread = hasOwnThread;

    window = SDL_CreateWindow(name.c_str(),
                              position.x, 
                              position.y, 
                              windowSize.width, windowSize.height, mode);

    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE; 
    context = SDL_CreateRenderer(window, -1, render_flags);
    if(context == NULL) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error initializing context", SDL_GetError(), NULL);

    this->name = name;
  }

  MTR::Window* MTR::Window::getWindow(const std::string& windowName){return windows[windowName];};
  // std::map<std::string, Window*> MTR::Window::getWindows(){return windows;};

  std::string Window::newWindow(
    int layerCount,
    Box windowSize,
    const std::string& name,
    Vec2 position,
    bool hasOwnThread
    // SDL_WindowFlags mode
  ){
    Window* win = new Window{
     layerCount,
     windowSize,
     name,
     position,
     hasOwnThread,
     (SDL_WindowFlags) 0
    };
    windows.emplace(name, win);

    if(windows.size() == 1) {
      currentWindow = name;
    }

    if(hasOwnThread){ 
      std::thread(mane, win).detach();
      // win->renderThread.detach();
    }
    else openMulti();

    return name;
  }

  // THIS DOES NOT WORKS!!
  void Window::openMulti(){
    if(!multiInitialized) {
      multiInitialized = true;
      // std::thread(multiMane).detach();
    }
  };

  void Window::closeMulti(){
    multiInitialized = false;
  };

  void Window::mane(){
    while(!pendingErase) {
      bool hasChanges = true;

      mutex.lock();
      // Buffer::moveBuffer(&buffer.swapBuffer, &buffer.readBuffer);

      buffer.readBuffer.dfrLayer.merge(buffer.writeBuffer.dfrLayer);
      for(int i = 0; i < buffer.writeBuffer.dfrSpriteGroup.size(); i++){

        if(i >= buffer.readBuffer.dfrSpriteGroup.size()) {
          buffer.readBuffer.dfrSpriteGroup.push_back(std::map<void*, MTR::RND::SpriteGroup>());
        }

        buffer.readBuffer.dfrSpriteGroup[i].merge(buffer.writeBuffer.dfrSpriteGroup[i]);
        // if(!buffer.readBuffer.dfrSpriteGroup[i].empty()){
        //   break;
        // }

      }
      buffer.readBuffer.dfrSuperGroup.merge(buffer.writeBuffer.dfrSuperGroup);

      mutex.unlock();
      if(hasChanges) windows[name]->display();
    }
    delete windows[name];
    windows.erase(name);
  }

  void Window::multiMane(){
    std::map<std::string, Window*>::iterator it;
    // bool breakOutOfWhile = false;
    while (multiInitialized) {
      if(windows.size() != 0) {
        int otherThread = 0;
        for(it = windows.begin(); it != windows.end(); it++){

          if (it->second == nullptr ||
              it->second->pendingErase) {

            delete windows[name];
            it = windows.erase(it);
            if(windows.size() == 0) {
              closeMulti();
            }
            continue;

          } else if (it->second->getHasOwnThread()) otherThread++;
          else {

            multimutex.lock();
            it->second->buffer.readBuffer.dfrLayer = it->second->buffer.writeBuffer.dfrLayer;
            it->second->buffer.readBuffer.dfrSpriteGroup = it->second->buffer.writeBuffer.dfrSpriteGroup;
            it->second->buffer.readBuffer.dfrSuperGroup = it->second->buffer.writeBuffer.dfrSuperGroup;
            // Buffer::moveBuffer(
            //   &windows[it->first]->buffer.swapBuffer,
            //   &windows[it->first]->buffer.readBuffer);
            multimutex.unlock();

            // windows[it->first]->buffer.
            it->second->display();
          }
          it++;
        }
        if(otherThread == windows.size()) closeMulti();
      }
    }
  }

  void Window::display(){
    SDL_RenderClear(context);
    // DeferBuffer buffer.readBuffer = buffer.readBuffer;

    std::map<void*, MTR::RND::Layer      >::iterator Lit;
    // std::map<void*, FK::AT ::SuperGroup  >::iterator SupGit;
    std::map<void*, MTR::RND::SpriteGroup>::iterator SpGit;
    std::map<void*, MTR::RND::Image      >::iterator Iit;

    // MTR::RND::Layer cLayer;
    void* cSupG = nullptr;

    // TODO :: Cheack readbuffer layer quantity against
    // writebuffer's and expand readbuffer accordingly

    // FK::AT::Renderable cR;
    // buffer.readBuffer
    int layer = 0;
    Lit = buffer.readBuffer.dfrLayer.begin();
    while(Lit != buffer.readBuffer.dfrLayer.end()){
      // this should contain only one layer
      // for(Lit = buffer.readBuffer.dfrLayer[layer].begin(); Lit != buffer.readBuffer.dfrLayer[layer].end(); Lit++){
        // cLayer = Lit->second;
      // Lit->second = Lit->second;

      // }
      // if(Lit->second.pendingErase){

      // }
      if(Lit->second.hidden) continue;
      if(!buffer.readBuffer.dfrSpriteGroup[layer].empty()){

        SpGit  = buffer.readBuffer.dfrSpriteGroup[layer].begin();
        while(
            SpGit != buffer.readBuffer.dfrSpriteGroup[layer].end()){
          // REVIEW THIS
          // If the spriteGroup has a pending erase, delete all sprite members
          if(SpGit->second.pendingErase){
            // SpGit->second.spriteDfr.clear();

            SpGit = buffer.readBuffer.dfrSpriteGroup[layer].erase(SpGit);

            continue;
          }

          if(SpGit->second.hidden) {
            SpGit++;
            continue;
          }
          // cSpG  = Rit->second.ownerGroup;
          Vec2 superOffsets = {0,0};
          if(buffer.readBuffer.dfrSuperGroup.find(SpGit->second.ownerGroup) != buffer.readBuffer.dfrSuperGroup.end()){
            cSupG = SpGit->second.ownerGroup;
          } else cSupG = nullptr;

          bool supGHidden = false;
          while(cSupG != nullptr){
            std::map<void *, MTR::RND::SuperGroup>::iterator supGit = buffer.readBuffer.dfrSuperGroup.find(cSupG);
            if(supGit!=buffer.readBuffer.dfrSuperGroup.end()){
              if(supGit->second.hidden) {
                supGHidden = true;
                break;
              }
              superOffsets.x += supGit->second.bounds.pos.x;
              superOffsets.y += supGit->second.bounds.pos.y;
              cSupG           = supGit->second.ownerGroup;
            } else break;
          }
          if(cSupG != nullptr && supGHidden){
            SpGit++;
            continue;
          }   


          Iit  = SpGit->second.spriteDfr.begin();
          while(
            Iit != SpGit->second.spriteDfr.end()
            // ;Iit++
            ){
            if(Iit->second.pendingErase){
              // for(SDL_Texture* tex : )
              if(Iit->second.texture != nullptr) SDL_DestroyTexture(Iit->second.texture);
              // }
              Iit = SpGit->second.spriteDfr.erase(Iit);
              continue;
            } else if(Iit->second.layer != layer){
              Iit = SpGit->second.spriteDfr.erase(Iit);
              continue;
            }
            if(Iit->second.hidden) {
              Iit++;
              continue;
            }
            if(Iit->second.surfaceUpdated){

              Iit->second.texture = SDL_CreateTextureFromSurface(Window::getWindow(name)->getContext(), SUR::surfaces[Iit->second.fileName].sur);
              if(Iit->second.texture == nullptr) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading image", SDL_GetError(), NULL);

              if(!Iit->second.surfaceFrSpriteSheet){
                SDL_QueryTexture(Iit->second.texture, NULL, NULL, &Iit->second.area.box.width, &Iit->second.area.box.height);
              }
              Iit->second.surfaceUpdated = false;

            }

            Vec2 screenPosCam = {0,0};
            if(!SpGit->second.ignoreCamera){
              screenPosCam = 
              {SpGit->second.worldPos.x - camPos.x,
              SpGit->second.worldPos.y - camPos.y};
            }

            Vec2 newPos = {
              Lit->second.bounds.pos.x                              +
              Iit->second.bounds.pos.x                         + 
              SpGit->second.bounds.pos.x                       +
              superOffsets.x                                   +
              screenPosCam.x,

              Lit->second.bounds.pos.y                              +
              Iit->second.bounds.pos.y                         + 
              SpGit->second.bounds.pos.y                       +
              superOffsets.y                                   +
              screenPosCam.y
              };
            
            // Crude culling
            // if((newPos.x + Iit->second.bounds.box.width  < 0 &&
            //     newPos.y + Iit->second.bounds.box.height < 0) ||
            //     (newPos.x > bounds.box.width && newPos.y > bounds.box.height)){
            //   Iit++;
            //   continue;
            // }

            Iit->second.tgtRect.x = newPos.x;
            Iit->second.tgtRect.y = newPos.y;

            bool hasRotCenter = false;
            Vec2 rotCenter = Iit->second.rotCenter;
            if(rotCenter.x != INT_MAX && rotCenter.y != INT_MAX){
              hasRotCenter = true;
            }
            
            somethingToDraw = true;
            SDL_RenderCopyEx(context,
            Iit->second.texture,
            &(Iit->second.srcRect), &(Iit->second.tgtRect),
            Iit->second.angle,
            hasRotCenter ? (SDL_Point*) &(rotCenter) : nullptr,
            (SDL_RendererFlip) Iit->second.flip);

            Iit++;
            
          }
          SpGit++;
        }
      }
      layer++;
      Lit++;
    }

    if(somethingToDraw) {
      SDL_RenderPresent(context);
    }
    somethingToDraw = false;
  }

  void Window::updateAll(){
    std::map<std::string, Window*>::iterator it;

    for(it = windows.begin(); it != windows.end(); it++){
      if(it->second->hasOwnThread) it->second->mutex.lock();
      else multimutex.lock();

      Buffer::addUpdates(&it->second->buffer.writeBuffer, &it->second->buffer.writeBuffer);

      if(it->second->hasOwnThread) it->second->mutex.unlock();
      else multimutex.unlock();

    }
  }

  void Window::updateOne(const std::string& window){
    if(windows[window]->hasOwnThread) windows[window]->mutex.lock();
    else multimutex.lock();

    // windows[window]->buffer.writeBuffer << std::move(windows[window]->buffer.writeBuffer);
    Buffer::addUpdates(&windows[window]->buffer.writeBuffer, &windows[window]->buffer.writeBuffer);

    // Buffer::cleanBuffer(&(windows[window]->buffer.writeBuffer));

    if(windows[window]->hasOwnThread) windows[window]->mutex.unlock();
    else multimutex.unlock();

  }

}