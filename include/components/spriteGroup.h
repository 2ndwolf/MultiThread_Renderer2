#ifndef _MTR_SPRITEGROUP_h_
#define _MTR_SPRITEGROUP_h_

#include <vector>
#include <atomic>

#include "image.h"
#include "superGroup.h"
#include "renderable.h"
#include "window.h"


namespace MTR{

  namespace RND{

    class SpriteGroup : public SuperGroup {
      public:
      int layer = 0;
      bool ignoreCamera = true ;
      Vec2 worldPos     = {0,0};


      inline SpriteGroup(){};

      // inline SpriteGroup(std::map<void*, MTR::RND::Image> sprites){
      //   updateSprites(sprites);
      // };

      inline ~SpriteGroup(){
        std::map<void*, MTR::RND::Image*>::iterator it;
        for(it = sprites.begin(); it != sprites.end(); it++){
          it->second->pendingErase = true;
          it->second->update(it->second) ;
          // delete sprites[i];
        }
      };

      static void update(SpriteGroup* group);

      // inline void updateSprites(std::map<void*, FK::AT::Image> sprites){
      //   std::map<void*, FK::AT::Image>::iterator it;
      //   for(it = sprites.begin(); it != sprites.end(); it++){
      //     this->sprites.emplace(it->first, &it->second);
      //   }
      //   // setUpdate();
      // }

      void refreshBounds(Renderable* rend){
        Vec2 oldPos = bounds.pos;
        SuperGroup::refreshBounds(rend);

        if(bounds.pos != oldPos && !ignoreCamera){
          worldPos.x -= bounds.pos.x - oldPos.x;
          worldPos.y -= bounds.pos.y - oldPos.y;
        }
      }

      inline void placeSprites(std::vector<MTR::RND::Image*> spr){
        for(int i = 0; i < spr.size(); i++){
          placeSprite(spr[i]);
        }
      }
      inline void placeSprite(MTR::RND::Image* spr){
        sprites[&spr] = spr;
      }
      
      inline void placeSprite(void* ptr, MTR::RND::Image* spr){
        sprites.emplace(ptr, spr);
      }

      // inline static void deepCopy(SpriteGroup* source, SpriteGroup* target){
      //   if(source->toUpdate){
      //     target->sprites = source->sprites;
      //     SuperGroup::deepCopy((SuperGroup*) source, target);
      //   }

      std::map<void*, MTR::RND::Image*> sprites;
      // protected:
      // bool initialized = false;
    };

    // std::map<void*, SuperGroup> updates = std::map<void*, SuperGroup>();
  }
}
      
#endif