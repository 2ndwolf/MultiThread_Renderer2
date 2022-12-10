#include <vector>
#include <map>
#include <algorithm>

#include "spriteGroup.h"
#include "superGroup.h"
#include "image.h"
#include "layer.h"

#include "buffer.h"
#include "renderUpdates.h"

namespace MTR{


  void Buffer::moveBuffer(RenderUpdates* source, RenderUpdates* target){

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
            std::map<void*, MTR::RND::Image*      >::iterator it;
            for(it  = source->updSprite->upd[layer].begin();
                it != source->updSprite->upd[layer].end  (); it++){

                MTR::RND::Image* img = new MTR::RND::Image();
                MTR::RND::Image::deepCopy(it->second, img);

                if(
                  target->updSpriteGroup->upd[layer].find((void*)&(it->second->ownerGroup))
                  == target->updSpriteGroup->upd[layer].end())
                  {
                  target->updSpriteGroup->upd[layer].
                  emplace((void*)&(it->second->ownerGroup), new MTR::RND::SuperGroup());
                  }

                if(
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites.find(it->first) !=
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites.end())

                  { delete
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites[it->first];
                  }


                target->updSpriteGroup->upd[layer]
                [(void*)&(it->second->ownerGroup)]->
                placeSprite(it->first, img);

            }
            source->updSprite->upd[layer].clear();
          }
          break;
          case RenderUpdates::updType::TEXT:
          {
            std::map<void*, MTR::RND::Text*      >::iterator it;
            for(it  = source->updText->upd[layer].begin();
                it != source->updText->upd[layer].end  (); it++){

                MTR::RND::Text* txt = new MTR::RND::Text();
                MTR::RND::Text::deepCopy(it->second, txt);

                if(
                  target->updSpriteGroup->upd[layer].find((void*)&(it->second->ownerGroup))
                  == target->updSpriteGroup->upd[layer].end())
                  {
                  target->updSpriteGroup->upd[layer].
                  emplace((void*)&(it->second->ownerGroup), new MTR::RND::SuperGroup());
                  }

                if(
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites.find(it->first) !=
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites.end())

                  { delete
                  target->updSpriteGroup->upd[layer]
                  [(void*)&(it->second->ownerGroup)]->sprites[it->first];
                  }


                target->updSpriteGroup->upd[layer]
                [(void*)&(it->second->ownerGroup)]->
                placeSprite(it->first, txt);


                // target->updSpriteGroup->upd[layer]
                // [(void*)&(it->second.ownerGroup)].
                // placeSprite(it->first, &(it->second));

            }
            source->updText->upd[layer].clear();
          }

          break;
          case RenderUpdates::updType::SPRITEGROUP:
          { 
            std::map<void*, MTR::RND::SpriteGroup*      >::iterator it;
            for(it  = source->updSpriteGroup->upd[layer].begin();
                it != source->updSpriteGroup->upd[layer].end()  ; it++){
                MTR::RND::SpriteGroup* sG = new MTR::RND::SpriteGroup();
                MTR::RND::SpriteGroup::deepCopy(it->second, sG);
                sG->sprites = target->updSpriteGroup->upd[layer][it->first]->sprites;

                if(
                  target->updSpriteGroup->upd[layer].find(it->first) !=
                  target->updSpriteGroup->upd[layer].end())
                  { delete
                  target->updSpriteGroup->upd[layer][it->first];
                  }

                target->updSpriteGroup->upd[layer][it->first] = sG;

            }
            source->updSpriteGroup->upd[layer].clear();
          }
          break;
          case RenderUpdates::updType::SUPERGROUP:
          { 
            std::map<void*, MTR::RND::SuperGroup*      >::iterator it;
            for(it  = source->updSuperGroup->upd.begin();
                it != source->updSuperGroup->upd.end()  ; it++){

                MTR::RND::SuperGroup* supG = new MTR::RND::SuperGroup();
                MTR::RND::SuperGroup::deepCopy(it->second, supG);

                if(
                  target->updSuperGroup->upd.find(it->first) !=
                  target->updSuperGroup->upd.end())
                  { delete
                  target->updSuperGroup->upd[it->first];
                  }

                target->updSuperGroup->upd  [it->first] = supG;

            }

            source->updSuperGroup->upd.clear();
          }
          
          break;

        }
      }
    }
  }
}
