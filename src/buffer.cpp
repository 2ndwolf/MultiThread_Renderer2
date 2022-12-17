#include <vector>
#include <map>
#include <algorithm>

#include "spriteGroup.h"
#include "superGroup.h"
#include "image.h"
#include "layer.h"

#include "buffer.h"
// #include "renderUpdates.h"

namespace MTR{


  void Buffer::addUpdates(WriteBuffer* source, DeferBuffer* target){
    // TODO :: CHECK LAYER QUANTITY (vector sizes)
    for(int i = 0; i < RenderUpdates::updType::LENGTH; i++){
      switch(i){
        case RenderUpdates::updType::LAYER:
        {
          // std::map<void*, MTR::RND::Layer      >::iterator Lit;

          // Lit = source->dfrLayer.begin();
          // while(Lit != source->dfrLayer.end()){
          // std::map<void*, MTR::RND::Layer*>::iterator it;
          // for(it  = source->ptrLayer.begin();
          //   it != source->ptrLayer.end()  ; it++){
          //   // if(Lit->second.isDue){
              // target->dfrLayer = source->dfrLayer;
          std::map<void*, MTR::RND::Layer*>::iterator it;
          for(it  = source->ptrLayer.begin();
              it != source->ptrLayer.end()  ; it++){


              target->dfrLayer.emplace(it->second->self, *it->second);
          }
              // target->dfrLayer[Lit->first] = RND::Layer(std::move(Lit->second));
            // }
            // Lit++;
          // }
          source->ptrLayer.clear();
        }
        break;
        case RenderUpdates::updType::SPRITEGROUP:
        for(int layer = 0; layer < source->dfrSpriteGroup.size(); layer++)
        { 
          if(layer >= target->dfrSpriteGroup.size()) {
            target->dfrSpriteGroup.push_back(std::map<void*, MTR::RND::SpriteGroup>());
          }

          std::map<void*, MTR::RND::SpriteGroup*>::iterator it;
          for(it  = source->ptrSpriteGroup[layer].begin();
              it != source->ptrSpriteGroup[layer].end()  ; it++){

              void* bob = it->first;
              target->dfrSpriteGroup[layer].emplace(it->second->self, *it->second);
          }
          source->ptrSpriteGroup[layer].clear();
        }
        break;
        case RenderUpdates::updType::SUPERGROUP:
        { 
          std::map<void*, MTR::RND::SuperGroup*>::iterator it;
          for(it  = source->ptrSuperGroup.begin();
              it != source->ptrSuperGroup.end()  ; it++){

              // if((it->second).pendingErase){
              //   target->dfrSuperGroup.erase(it);
              //   continue;
              // }

              target->dfrSuperGroup.emplace(it->second->self, *it->second);
          }
          // target->dfrSuperGroup.merge(source->dfrSuperGroup);
          source->ptrSuperGroup.clear();
        }
        
        break;

      }
    }
  }
}
  
  // void Buffer::cleanBuffer(RenderUpdates* buffer){
  //   // std::map<void *, MTR::RND::SpriteGroup *>::iterator spGit;
  //   // std::map<void *, MTR::RND::SuperGroup *>::iterator supGit;

  //   for(int i = 0; i < buffer->updLayer.size(); i++){
  //     // updLayer[i].clear();

  //     // for(spGit = buffer->updSpriteGroup[i].begin();
  //     //     spGit != buffer->updSpriteGroup[i].end(); spGit++){
  //     //     // delete spGit->second;
  //     // }
  //     buffer->updSpriteGroup[i].clear();
  //   }

  //   // for(supGit  = buffer->updSuperGroup.begin();
  //   //     supGit != buffer->updSuperGroup.end(); supGit++){
  //   //     // delete supGit->second;
  //   // }
  //   buffer->updSuperGroup.clear();

  // }
