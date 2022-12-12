#include <thread>
#include <algorithm>
#include <map>
#include <string>
#include <atomic>

#include <SDL.h>

#include "buffer.h"
#include "primitives.h" 
#include "window.h"
#include "layer.h"
#include "superGroup.h"
#include "spriteGroup.h"
#include "image.h"

namespace MTR{

  Window::Window(
          int layerCount,
          Box windowSize,
          std::string name,
          bool hasOwnThread,
          Vec2 position,
          SDL_WindowFlags mode
          ){

    buffer = Buffer(layerCount);
    bounds = {position, windowSize};
    this->hasOwnThread = hasOwnThread;

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
    Vec2 position
    // SDL_WindowFlags mode
  ){
    Window* win = new Window(
     layerCount,
     windowSize,
     name,
     hasOwnThread,
     position,
     (SDL_WindowFlags) 0
    );
    windows.insert({name, win});

    if(hasOwnThread) win->renderThread = std::thread(mane, win);
    else openMulti();

    return name;
  }


  void Window::openMulti(){
    if(!multiInitialized) {
      multiInitialized = true;
      std::thread(multiMane);
    }
  };

  void Window::closeMulti(){
    multiInitialized = false;
  };

  void Window::mane(){
    while(!pendingErase) {

      mutex.lock();
      Buffer::moveBuffer(&buffer.swapBuffer, &buffer.readBuffer);
      mutex.unlock();

      windows[name]->display();
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
            Buffer::moveBuffer(
              &windows[it->first]->buffer.swapBuffer,
              &windows[it->first]->buffer.readBuffer);
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
    RenderUpdates rB = buffer.readBuffer;

    std::map<void*, MTR::RND::Layer       >::iterator Lit;
    // std::map<void*, FK::AT ::SuperGroup  >::iterator SupGit;
    std::map<void*, MTR::RND::SpriteGroup*>::iterator SpGit;
    std::map<void*, MTR::RND::Image      *>::iterator Iit;

    MTR::RND::Layer cLayer;
    void* cSupG;

    // FK::AT::Renderable cR;
    // buffer.readBuffer
    for(int layer = 0; layer < rB.updLayer->upd.size(); layer++){
      // this should contain only one layer
      // for(Lit = rB.updLayer->upd[layer].begin(); Lit != rB.updLayer->upd[layer].end(); Lit++){
        // cLayer = Lit->second;
        cLayer = rB.updLayer->upd[layer];
      // }
      if(cLayer.hidden) continue;

      SpGit  = rB.updSpriteGroup->upd[layer].begin();
      while(
          SpGit != rB.updSpriteGroup->upd[layer].end()){
        // REVIEW THIS
        if(rB.updSpriteGroup->upd[layer][SpGit->first]->pendingErase){
          std::map<void*, MTR::RND::Image*>::iterator fIT;
          for(fIT = rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.begin();
          fIT != rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.end();
          fIT++) delete fIT->second;
          rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.clear();

          delete SpGit->second;
          SpGit = rB.updSpriteGroup->upd[layer].erase(SpGit);

          continue;
        }

        if(rB.updSpriteGroup->upd[layer][SpGit->first]->hidden) {
          SpGit++;
          continue;
        }
        // cSpG  = Rit->second.ownerGroup;
        Vec2 superOffsets = {0,0};
        cSupG = rB.updSuperGroup->upd[SpGit->first]->ownerGroup;

        while(cSupG != nullptr){
          if(rB.updSuperGroup->upd[cSupG]->hidden) break;
          superOffsets.x += rB.updSuperGroup->upd[cSupG]->bounds.pos.x;
          superOffsets.y += rB.updSuperGroup->upd[cSupG]->bounds.pos.y;
          cSupG           = rB.updSuperGroup->upd[cSupG]->ownerGroup;
        }
        if(rB.updSuperGroup->upd[cSupG]->hidden){
          SpGit++;
          continue;
        } 


        Iit  = rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.begin();
        while(
          Iit != rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.end  ()
          // ;Iit++
          ){
          if(Iit->second->pendingErase){
            // for(SDL_Texture* tex : )
            SDL_DestroyTexture(rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs[Iit->first]->textures[name]);
            // }
            rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.erase(Iit->first);
            continue;
          }
          if(Iit->second->layer != layer){
            rB.updSpriteGroup->upd[layer][SpGit->first]->spritePTRs.erase(Iit->first);
            continue;
          }
          if(Iit->second->hidden) {
            Iit++;
            continue;
          }

          Vec2 screenPosCam = {0,0};
          if(!rB.updSpriteGroup->upd[layer][SpGit->first]->ignoreCamera){
            screenPosCam = 
            {rB.updSpriteGroup->upd[layer][SpGit->first]->worldPos.x - camPos.x,
             rB.updSpriteGroup->upd[layer][SpGit->first]->worldPos.y - camPos.y};
          }

          Vec2 newPos = {
            cLayer.offset.x                                   +
            Iit->second->bounds.pos.x                         + 
            rB.updSuperGroup->upd[SpGit->first]->bounds.pos.x +
            superOffsets.x                                    +
            screenPosCam.x,

            cLayer.offset.y                                   +
            Iit->second->bounds.pos.y                         + 
            rB.updSuperGroup->upd[SpGit->first]->bounds.pos.y +
            superOffsets.y                                    +
            screenPosCam.y
            };
          
          // Crude culling
          if((newPos.x + Iit->second->bounds.box.width  < 0 &&
              newPos.y + Iit->second->bounds.box.height < 0) ||
              (newPos.x > bounds.box.width && newPos.y > bounds.box.height)){
            Iit++;
            continue;
          }

          Iit->second->tgtRect.x = newPos.x;
          Iit->second->tgtRect.y = newPos.y;

          bool hasRotCenter = false;
          Vec2 rotCenter = Iit->second->rotCenter;
          if(rotCenter.x != INT_MAX && rotCenter.y != INT_MAX){
            hasRotCenter = true;
          }
          
          SDL_RenderCopyEx(context,
          Iit->second->textures[name],
          &(Iit->second->srcRect), &(Iit->second->tgtRect),
          Iit->second->angle,
          hasRotCenter ? (SDL_Point*) &(rotCenter) : nullptr,
          (SDL_RendererFlip) Iit->second->flip);

          Iit++;
        }
        SpGit++;
      }
    }

    SDL_RenderPresent(context);
  }

  void Window::updateAll(){
    std::map<std::string, Window*>::iterator it;
    for(it = windows.begin(); it != windows.end(); it++){
      if(it->second->hasOwnThread) it->second->mutex.lock();
      else multimutex.lock();

      it->second->buffer.swapBuffer = it->second->buffer.writeBuffer;

      if(it->second->hasOwnThread) it->second->mutex.unlock();
      else multimutex.unlock();
    }
  }

  void Window::updateOne(std::string window){
    if(windows[window]->hasOwnThread) windows[window]->mutex.lock();
    else multimutex.lock();

    windows[window]->buffer.swapBuffer = windows[window]->buffer.writeBuffer;

    if(windows[window]->hasOwnThread) windows[window]->mutex.unlock();
    else multimutex.unlock();
  }

}