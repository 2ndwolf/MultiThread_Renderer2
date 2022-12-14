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

    for(int layer = 0; layer < source->updLayer.size(); layer++){
      for(int i = 0; i < RenderUpdates::updType::LENGTH; i++){
        switch(i){
          case RenderUpdates::updType::LAYER:
          { 
            // std::map<void*, FK::    Layer      >::iterator it;
            // for(it  = source->updLayer[layer].begin();
            //     it != source->updLayer[layer].end()  ; it++){
            //     target->updLayer[layer].emplace(it->first, it->second);
            // }
                      // target->updLayer[layer] = source->updLayer[layer];

            // source->updLayer.erase(layer);
            // source->updLayer[layer] 
          }
          break;
// {          // case RenderUpdates::updType::SPRITE:
//           // {
//           //   std::map<void*, MTR::RND::Image*      >::iterator it;
//           //   for(it  = source->updSprite[layer].begin();
//           //       it != source->updSprite[layer].end  (); it++){

//           //       MTR::RND::Image* img = new MTR::RND::Image();
//           //       MTR::RND::Image::deepCopy(it->second, img);

//           //       if(
//           //         target->updSpriteGroup[layer].find((void*)&(it->second->ownerGroup))
//           //         == target->updSpriteGroup[layer].end())
//           //         {
//           //         target->updSpriteGroup[layer].
//           //         emplace((void*)&(it->second->ownerGroup), new MTR::RND::SuperGroup());
//           //         }

//           //       if(
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites.find(it->first) !=
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites.end())

//           //         { delete
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites[it->first];
//           //         }


//           //       target->updSpriteGroup[layer]
//           //       [(void*)&(it->second->ownerGroup)]->
//           //       placeSprite(it->first, img);

//           //   }
//           //   source->updSprite[layer].clear();
//           // }
//           // break;
//           // case RenderUpdates::updType::TEXT:
//           // {
//           //   std::map<void*, MTR::RND::Text*      >::iterator it;
//           //   for(it  = source->updText[layer].begin();
//           //       it != source->updText[layer].end  (); it++){

//           //       MTR::RND::Text* txt = new MTR::RND::Text();
//           //       MTR::RND::Text::deepCopy(it->second, txt);

//           //       if(
//           //         target->updSpriteGroup[layer].find((void*)&(it->second->ownerGroup))
//           //         == target->updSpriteGroup[layer].end())
//           //         {
//           //         target->updSpriteGroup[layer].
//           //         emplace((void*)&(it->second->ownerGroup), new MTR::RND::SuperGroup());
//           //         }

//           //       if(
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites.find(it->first) !=
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites.end())

//           //         { delete
//           //         target->updSpriteGroup[layer]
//           //         [(void*)&(it->second->ownerGroup)]->sprites[it->first];
//           //         }


//           //       target->updSpriteGroup[layer]
//           // //       [(void*)&(it->second->ownerGroup)]->
//           //       placeSprite(it->first, txt);


//           //       // target->updSpriteGroup[layer]
//           //       // [(void*)&(it->second.ownerGroup)].
//           //       // placeSprite(it->first, &(it->second));

//           //   }
//           //   source->updText[layer].clear();
//           // }
//           // break;}
          case RenderUpdates::updType::SPRITEGROUP:
          { 
            std::map<void*, MTR::RND::SpriteGroup*      >::iterator it;
            for(it  = source->updSpriteGroup[layer].begin();
                it != source->updSpriteGroup[layer].end()  ; it++){
                MTR::RND::SpriteGroup* sG = new MTR::RND::SpriteGroup(
                  it->second->windows
                );
                MTR::RND::SpriteGroup::deepCopy(it->second, sG);
                // sG->sprites = target->updSpriteGroup[layer][it->first]->sprites;

                // if(
                //   target->updSpriteGroup[layer].find(it->first) !=
                //   target->updSpriteGroup[layer].end())
                //   { delete
                //   target->updSpriteGroup[layer][it->first];
                //   }

                target->updSpriteGroup[layer][it->first] = sG;

                delete it->second;

            }
if(source->updSpriteGroup[layer].size() != 0)
            source->updSpriteGroup[layer].clear();
          }
          break;
          case RenderUpdates::updType::SUPERGROUP:
          { 
            std::map<void*, MTR::RND::SuperGroup*      >::iterator it;
            for(it  = source->updSuperGroup.begin();
                it != source->updSuperGroup.end()  ; it++){

                MTR::RND::SuperGroup* supG = new MTR::RND::SuperGroup(
                  it->second->windows
                );
                MTR::RND::SuperGroup::deepCopy(it->second, supG);

                if(
                  target->updSuperGroup.find(it->first) !=
                  target->updSuperGroup.end())
                  { delete
                  target->updSuperGroup[it->first];
                  }

                target->updSuperGroup[it->first] = supG;

                delete source->updSpriteGroup[layer][it->first];

            }

            source->updSuperGroup.clear();
          }
          
          break;

        }
      }
    }
  }
}
