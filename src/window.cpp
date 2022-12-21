#include <thread>
#include <algorithm>
#include <map>
#include <string>
#include <atomic>
#include <utility>
#include <cmath>


#include <SDL.h>

#include "buffer.h"
#include "primitives.h" 
#include "window.h"
#include "layer.h"
#include "superGroup.h"
#include "spriteGroup.h"
#include "image.h"

#include "INST.h"

#include "defaults.h"

/**
 * 
 * Utilities for window.cpp
 * 
*/


Vec2 rotatePoint(Vec2 rotCenter, float angle, Vec2 point){
  float s = std::sin((angle/180)*3.14);
  float c = std::cos((angle/180)*3.14);

  point.x = point.x - rotCenter.x;
  point.y = point.y - rotCenter.y;

  int xnew = point.x * c - point.y * s;
  int ynew = point.x * s + point.y * c;

  point.x = xnew + rotCenter.x; 
  point.y = ynew + rotCenter.y; 

  return point;
}


/**
 * 
 * 
 * 
*/
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
      updateReadBuffer();
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

  std::unordered_map<void *, SDL_Texture*> dfrTexs;
  std::map<void*, MTR::RND::SpriteGroup>::iterator SpGit;
  std::map<void*, MTR::RND::Layer      >::iterator Lit;
  std::unordered_map<void *, SDL_Texture *>::iterator texIt;

  void Window::display(){
    // SDL_RenderClear(context);
    // DeferBuffer buffer.readBuffer = buffer.readBuffer;

    // // std::map<void*, FK::AT ::SuperGroup  >::iterator SupGit;
    // // std::map<void*, MTR::RND::Image      >::iterator Iit;






    

    // int Iit = 0;

    // // MTR::RND::Layer cLayer;
    void* cSupG ;
    bool somethingToDraw = false;

    // TODO :: Cheack readbuffer layer quantity against
    // writebuffer's and expand readbuffer accordingly

    // FK::AT::Renderable cR;
    // buffer.readBuffer
    // int layer = 0;
    // Lit = buffer.readBuffer.dfrLayer.begin();
    // while(Lit != buffer.readBuffer.dfrLayer.end()){
    //   // this should contain only one layer
    //   // for(Lit = buffer.readBuffer.dfrLayer[layer].begin(); Lit != buffer.readBuffer.dfrLayer[layer].end(); Lit++){
    //     // cLayer = Lit->second;
    //   // Lit->second = Lit->second;

    //   // }
    //   // if(Lit->second.pendingErase){

    //   // }
    //   if(Lit->second.hidden) continue;
      for(int layr = 0;  layr < buffer.readBuffer.dfrSpriteGroup.size(); layr++){
        // int layr = 0;
        // if(!buffer.readBuffer.dfrSpriteGroup[layr].empty()){
        SpGit = buffer.readBuffer.dfrSpriteGroup[layr].begin();
        while(
            SpGit != buffer.readBuffer.dfrSpriteGroup[layr].end()){
            // MTR::RND::Renderable spG = SpGit->second;
          // REVIEW THIS
          // If the spriteGroup has a pending erase, delete all sprite members
          if(SpGit->second.pendingErase){
            SpGit->second.spriteDfr.clear();

            SpGit = buffer.readBuffer.dfrSpriteGroup[layr].erase(SpGit);

            continue;
        
          }
          if(SpGit->second.hidden) {
            SpGit++;
            continue;
          } 
          // Vec2 superOffsets = {0,0};
          Vec2 superRots = {0,0};
          float zoomX = 1.00;
          float zoomY = 1.00;
          cSupG = SpGit->second.ownerGroup;

          bool supGHidden = false;

          Vec2 rotCenter = SpGit->second.rotCenter;

          std::vector<MTR::RND::SuperGroup> rotPoints = std::vector<MTR::RND::SuperGroup>();

          // superOffsets = rotatePoint(SpGit->second.rotCenter, SpGit->second.angle, superRots);
          // int newAngle = SpGit->second.angle * 
          //                std::pow((std::pow(SpGit->second.rotCenter.x - SpGit->second.bounds.box.width / 2, 2) +
          //                std::pow(SpGit->second.rotCenter.y - SpGit->second.bounds.box.height / 2, 2)),0.5);
          int newAngle = 0;
          
          while(cSupG != nullptr){
            std::map<void *, MTR::RND::SuperGroup>::iterator supGit = buffer.readBuffer.dfrSuperGroup.find(cSupG);

              if(supGit->second.hidden) {
                supGHidden = true;
                break;
              }
              zoomX *= supGit->second.zoom.x / 100.0;
              zoomY *= supGit->second.zoom.y / 100.0;
              rotPoints.push_back(supGit->second);

              // superOffsets.x += supGit->second.bounds.pos.x;
              // superOffsets.y += supGit->second.bounds.pos.y;


              // superRots = rotatePoint(supGit->second.rotCenter, supGit->second.angle, superRots);
              // rotCenter.x = supGit->second.rotCenter.x - rot

              // if(SpGit->second.angleFrSuper){
              //   newAngle += supGit->second.angle / 
              //   std::pow((std::pow(supGit->second.rotCenter.x - supGit->second.bounds.box.width / 2, 2) +
              //   std::pow(supGit->second.rotCenter.y - supGit->second.bounds.box.height / 2, 2)),0.5);
              // }

              cSupG           = supGit->second.ownerGroup;
            }
          
          if(cSupG != nullptr && supGHidden){
            SpGit++;
            continue;
          }   

          /// Erase_if
          int Iit  = 0;
          while(
            Iit < SpGit->second.spriteDfr.size()

            ){
            std::map<void *, MTR::RND::Image>::iterator Iitit = buffer.readBuffer.dfrImage.find(SpGit->second.spriteDfr[Iit]);
            if(Iitit != buffer.readBuffer.dfrImage.end()){

              if(Iitit->second.pendingErase){

                if(Iitit->second.texture != nullptr) SDL_DestroyTexture(Iitit->second.texture);

                continue;
              } 

              if(Iitit->second.hidden) {
                Iit++;
                continue;
              }
              if(Iitit->second.surfaceUpdated){
                  texIt = dfrTexs.find(Iitit->second.self);
                  if(texIt == dfrTexs.end()){
                    dfrTexs.emplace(Iitit->second.self,SDL_CreateTextureFromSurface(Window::getWindow(name)->getContext(), SUR::surfaces[Iitit->second.fileName].sur));

                    if(!Iitit->second.surfaceFrSpriteSheet){
                      SDL_QueryTexture(Iitit->second.texture, NULL, NULL, &Iitit->second.area.box.width, &Iitit->second.area.box.height);
                    }
                  } else {
                    dfrTexs.erase(texIt);
                    dfrTexs.emplace(Iitit->second.self, SDL_CreateTextureFromSurface(Window::getWindow(name)->getContext(), SUR::surfaces[Iitit->second.fileName].sur));
                  }
              }

              Vec2 screenPosCam = {0,0};
              if(!SpGit->second.ignoreCamera){
                screenPosCam = 
                {SpGit->second.worldPos.x - camPos.x,
                SpGit->second.worldPos.y - camPos.y};
              }

              Vec2 superOffsets = SpGit->second.bounds.pos ;
              superOffsets.x +=  Iitit->second.bounds.pos.x;
              superOffsets.y +=  Iitit->second.bounds.pos.y;


              int newAngle = 0;
              for(int k = rotPoints.size() - 1; k >= 0; k--){
                rotPoints[k].rotCenter = {250,250};
                superOffsets.x += rotPoints[k].bounds.pos.x;
                superOffsets.y += rotPoints[k].bounds.pos.y;
                // superOffsets = rotatePoint({rotPoints[k].rotCenter.x - rotPoints[k].bounds.box.width/2, rotPoints[k].rotCenter.y - rotPoints[k].bounds.box.height/2}, rotPoints[k].angle, superOffsets);

                if(Iitit->second.angleFrSuper){
                  newAngle += rotPoints[k].angle;
                }

              }

              // Crude culling
              // if((newPos.x + Iitit->second.bounds.box.width  < 0 &&
              //     newPos.y + Iitit->second.bounds.box.height < 0) ||
              //     (newPos.x > bounds.box.width && newPos.y > bounds.box.height)){
              //   Iit++;
              //   continue;
              // }

            Vec2 newPos = {
              (int)superOffsets.x                                   +
              superRots.x +
              screenPosCam.x,

              (int)superOffsets.y                                   +
              superRots.y + 
              screenPosCam.y
              };

              if(Iitit->second.zoom.x != 100){
                newPos.x = newPos.x - (int)((float)((newPos.x - Iitit->second.zoomCenter.x) * (zoomX * (Iitit->second.zoom.x / 100.0))));
                Iitit->second.tgtRect.w = (int)((float)(Iitit->second.area.box.width * (zoomX * (Iitit->second.zoom.x / 100.0))));
              }

              if(Iitit->second.zoom.y != 100){
                newPos.y = newPos.y - (int)((float)((newPos.y - Iitit->second.zoomCenter.y) * (zoomY * (Iitit->second.zoom.y / 100.0))));
                Iitit->second.tgtRect.h = (int)((float)(Iitit->second.area.box.height * (zoomY * (Iitit->second.zoom.y / 100.0))));
              }

              Iitit->second.tgtRect.x = newPos.x;
              Iitit->second.tgtRect.y = newPos.y;
              
              // Vec2 rotCenter = Iitit->second.rotCenter;
              bool hasRotCenter = (rotCenter.x != INT_MAX && rotCenter.y != INT_MAX);


              somethingToDraw = true;
              SDL_RenderCopyEx(context,
              dfrTexs.find(Iitit->second.self)->second,
              &(Iitit->second.srcRect), &(Iitit->second.tgtRect),
              Iitit->second.angleFrSuper ? newAngle + Iitit->second.angle : Iitit->second.angle,
              hasRotCenter ? (SDL_Point*) &(rotCenter) : nullptr,
              (SDL_RendererFlip) Iitit->second.flip);
            }
            Iit++;
            
          }
          SpGit++;
        }
      }
      // layr++;
      // Lit++;
    // }

    if(somethingToDraw) {
      SDL_RenderPresent(context);
    }
    // somethingToDraw = false;
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

  void Window::expandLayersIf(const int& layers){
    while (buffer.writeBuffer.ptrSpriteGroup.size() <= layers && buffer.writeBuffer.ptrSpriteGroup.size() <= MAXLAYERS){
      buffer.writeBuffer.ptrSpriteGroup.push_back(std::map<void*, MTR::RND::SpriteGroup*>());
    }
  }

  void Window::updateReadBuffer(){
    std::map<void *, MTR::RND::Layer>::iterator layIt ;
    std::map<void *, MTR::RND::Layer>::iterator layIt2;

    std::map<void *, MTR::RND::SpriteGroup>::iterator spIt ;
    std::map<void *, MTR::RND::SpriteGroup>::iterator spIt2;

    std::map<void *, MTR::RND::Image>::iterator imgIt ;
    std::map<void *, MTR::RND::Image>::iterator imgIt2;

    std::map<void *, MTR::RND::SuperGroup>::iterator supGIt ;
    std::map<void *, MTR::RND::SuperGroup>::iterator supGIt2;
    
    for(layIt = buffer.writeBuffer.dfrLayer.begin(); layIt != buffer.writeBuffer.dfrLayer.end(); layIt++){
      layIt2 = buffer.readBuffer.dfrLayer.find(layIt->second.self);
      if(layIt2 != buffer.readBuffer.dfrLayer.end()){
        buffer.readBuffer.dfrLayer.erase(layIt2);
      }
      buffer.readBuffer.dfrLayer.emplace(layIt->second.self, layIt->second);
    }

    for(int i = 0; i < buffer.writeBuffer.dfrSpriteGroup.size(); i++){

      while(buffer.readBuffer.dfrSpriteGroup.size() < buffer.writeBuffer.dfrSpriteGroup.size()) {
        buffer.readBuffer.dfrSpriteGroup.push_back(std::map<void*, MTR::RND::SpriteGroup>());
      }

      for(spIt = buffer.writeBuffer.dfrSpriteGroup[i].begin(); spIt != buffer.writeBuffer.dfrSpriteGroup[i].end(); spIt++){
        spIt2 = buffer.readBuffer.dfrSpriteGroup[i].find(spIt->second.self);
        if(spIt2 != buffer.readBuffer.dfrSpriteGroup[i].end()){
          buffer.readBuffer.dfrSpriteGroup[i].erase(spIt2);
        }
        buffer.readBuffer.dfrSpriteGroup[i].emplace(spIt->second.self, spIt->second);
      }

    }

    for(imgIt = buffer.writeBuffer.dfrImage.begin(); imgIt != buffer.writeBuffer.dfrImage.end(); imgIt++){
      imgIt2 = buffer.readBuffer.dfrImage.find(imgIt->second.self);
      if(imgIt2 != buffer.readBuffer.dfrImage.end()){
        buffer.readBuffer.dfrImage.erase(imgIt2);
      }
      buffer.readBuffer.dfrImage.emplace(imgIt->second.self, imgIt->second);
    }


    for(supGIt = buffer.writeBuffer.dfrSuperGroup.begin(); supGIt != buffer.writeBuffer.dfrSuperGroup.end(); supGIt++){
      supGIt2 = buffer.readBuffer.dfrSuperGroup.find(supGIt->second.self);
      if(supGIt2 != buffer.readBuffer.dfrSuperGroup.end()){
        buffer.readBuffer.dfrSuperGroup.erase(supGIt2);
      }
      buffer.readBuffer.dfrSuperGroup.emplace(supGIt->second.self, supGIt->second);
    }

  }

}