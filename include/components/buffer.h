#ifndef _MTR_BUFFER_h_
#define _MTR_BUFFER_h_

#include <vector>
#include <map>
#include <algorithm>

#include "spriteGroup.h"
#include "superGroup.h"
#include "image.h"
#include "layer.h"
#include "renderUpdates.h"

namespace MTR{

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
      for(int layer = 0; layer < source->updLayer->upd.size(); layer++){
        for(int i = 0; i < RenderUpdates::updType::LENGTH; i++){
          switch(i){
            case RenderUpdates::updType::LAYER:
            { 
              // std::map<void*, FK::    Layer      >::iterator it;
              // for(it  = source->updLayer->upd[layer].begin();
              //     it != source->updLayer->upd[layer].end()  ; it++){
              //     target->updLayer->upd[layer].emplace(it->first, it->second);
              // }
              target->updLayer->upd[layer] = source->updLayer->upd[layer];
              // source->updLayer->upd[layer] 
            }
           break;
            case RenderUpdates::updType::SPRITE:
            {
              std::map<void*, MTR::RND::Image      >::iterator it;
              for(it  = source->updSprite->upd[layer].begin();
                  it != source->updSprite->upd[layer].end  (); it++){
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second.ownerGroup)].
                  placeSprite(it->first, &(it->second));
              }
              source->updSprite->upd[layer].clear();
            }
            break;
            case RenderUpdates::updType::TEXT:
            {
              std::map<void*, MTR::RND::Text      >::iterator it;
              for(it  = source->updText->upd[layer].begin();
                  it != source->updText->upd[layer].end  (); it++){
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second.ownerGroup)].
                  placeSprite(it->first, &(it->second));
              }
              source->updText->upd[layer].clear();
            }

           break;
            case RenderUpdates::updType::SPRITEGROUP:
            { 
              std::map<void*, MTR::RND::SpriteGroup      >::iterator it;
              for(it  = source->updSpriteGroup->upd[layer].begin();
                  it != source->updSpriteGroup->upd[layer].end()  ; it++){
                  target->updSpriteGroup->upd[layer].emplace(it->first, it->second);
              }
              source->updSpriteGroup->upd[layer].clear();
            }
           break;
            case RenderUpdates::updType::SUPERGROUP:
            { 
              std::map<void*, MTR::RND::SuperGroup      >::iterator it;
              for(it  = source->updSuperGroup->upd.begin();
                  it != source->updSuperGroup->upd.end()  ; it++){
                  target->updSuperGroup->upd.emplace(it->first, it->second);
              }
              source->updSuperGroup->upd.clear();
            }
           break;

          }
        }
      }
    }
  };
}

#endif