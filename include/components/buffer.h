#ifndef MTR_BUFFER_h
#define MTR_BUFFER_h

#include <vector>
#include <map>
#include <algorithm>

#include "spriteGroup.h"
#include "superGroup.h"
#include "image.h"
#include "layer.h"

namespace FK{
  int layerCount;
  class Buffer{
    
    
    public:
    RenderUpdates writeBuffer;
    RenderUpdates readBuffer ;
    RenderUpdates swapBuffer ;

    Buffer(int layers){
      writeBuffer = RenderUpdates(layers);
      readBuffer  = RenderUpdates(layers);
      swapBuffer  = RenderUpdates(layers);
    }

    Buffer(){};

    inline static void moveBuffer(RenderUpdates* source, RenderUpdates* target){
      //FOR ALL WINDOWs
      //lock independent windows independently
      //lock multiMutex
      //
      // swapBuffer =
      for(int layer = 0; layer < layerCount; layer++){
        for(int i = 0; i < RenderUpdates::updType::LENGTH; i++){
          // it;
          UpdateType* src = source->checkUpdType((RenderUpdates::updType)i);
          UpdateType* tgt = target->checkUpdType((RenderUpdates::updType)i);
          switch(i){
            case RenderUpdates::updType::LAYER:
            { 
              std::map<void*, FK::    Layer      >::iterator it;
              for(it  = ((LayerUpdate*)src)->upd[layer].begin();
                  it != ((LayerUpdate*)src)->upd[layer].end()  ; it++){
                  ((LayerUpdate*)tgt)->upd[layer].emplace(it->first, it->second);
              }
              ((LayerUpdate*)src)->upd[layer].clear();
            }
           break;
            case RenderUpdates::updType::SPRITE:
            {
              std::map<void*, FK::AT::Image      >::iterator it;
              for(it  = ((SpriteUpdate*)src)->upd[layer].begin();
                  it != ((SpriteUpdate*)src)->upd[layer].end  (); it++){
                  // FK::AT::Sprite spr;
                  // spr.deepCopy(&it->second, &spr);
                  target->updSpriteGroup->upd[layer]
                  [it->second.ownerGroup].replaceSprite(it->first, &(it->second));
                  // ((SpriteUpdate*)tgt)->upd[layer].emplace();
              }
              ((SpriteUpdate*)src)->upd[layer].clear();
            }
            break;
            case RenderUpdates::updType::TEXT:
            {
              std::map<void*, FK::AT::Text      >::iterator it;
              for(it  = ((TextUpdate*)src)->upd[layer].begin();
                  it != ((TextUpdate*)src)->upd[layer].end  (); it++){
                  // FK::AT::Text spr;
                  // spr.deepCopy(&it->second, &spr);
                  target->updSpriteGroup->upd[layer]
                  [it->second.ownerGroup].replaceSprite(it->first, &(it->second));
                  // ((TextUpdate*)tgt)->upd[layer].emplace();
              }
              ((TextUpdate*)src)->upd[layer].clear();
            }

           break;
            case RenderUpdates::updType::SPRITEGROUP:
            { 
              std::map<void*, FK::AT::SpriteGroup      >::iterator it;
              for(it  = ((SpriteGroupUpdate*)src)->upd[layer].begin();
                  it != ((SpriteGroupUpdate*)src)->upd[layer].end()  ; it++){
                  ((SpriteGroupUpdate*)tgt)->upd[layer].emplace(it->first, it->second);
              }
              ((SpriteGroupUpdate*)src)->upd[layer].clear();
            }
           break;
            case RenderUpdates::updType::SUPERGROUP:
            { 
              std::map<void*, FK::AT::SuperGroup      >::iterator it;
              for(it  = ((SuperGroupUpdate*)src)->upd[layer].begin();
                  it != ((SuperGroupUpdate*)src)->upd[layer].end()  ; it++){
                  ((SuperGroupUpdate*)tgt)->upd[layer].emplace(it->first, it->second);
              }
              ((SuperGroupUpdate*)src)->upd[layer].clear();
            }
           break;

          }
        }
      }
    }

    void swapAllWrites(){
      // for(RenderUpdates wB: writeBuffers){
      //   swapBuffers(&wB, &swapBuffer);
      // }
    }
  };
}

#endif