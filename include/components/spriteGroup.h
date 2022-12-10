#ifndef _MTR_SPRITEGROUP_h_
#define _MTR_SPRITEGROUP_h_

#include <vector>
#include <atomic>

#include "image.h"
#include "superGroup.h"
#include "renderable.h"
// #include "window.h"


namespace MTR{

  namespace RND{

    class SpriteGroup : public SuperGroup {
      public:
      int layer         = 0;
      bool ignoreCamera = true ;
      Vec2 worldPos     = {0,0};

      static void deepCopy(SpriteGroup* source, SpriteGroup* target);

      inline SpriteGroup(){};

      static SpriteGroup gimmesG(){
        return SpriteGroup();
      }

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
      bool checkSanity();
      void refreshBounds(Renderable* rend);

      void placeSprites(std::vector<MTR::RND::Image*> spr);
      void placeSprite(MTR::RND::Image* spr);
      void placeSprite(void* ptr, MTR::RND::Image* spr);
      

      std::map<void*, MTR::RND::Image*> sprites;
      // protected:
      // bool initialized = false;
    };

    // std::map<void*, SuperGroup> updates = std::map<void*, SuperGroup>();
  }
}
      
#endif