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
    // class Window;

    class SpriteGroup : public SuperGroup {
      public:
      int layer         = 0;
      bool ignoreCamera = true ;
      Vec2 worldPos     = {0,0};

      static void deepCopy(SpriteGroup* source, SpriteGroup* target);

      SpriteGroup(std::vector<std::string> pwindows):
      SuperGroup(pwindows)
      {};

      // static SpriteGroup gimmesG(){
      //   return SpriteGroup();
      // }

      // inline SpriteGroup(std::map<void*, MTR::RND::Image> sprites){
      //   updateSprites(sprites);
      // };
      std::vector<MTR::RND::Image*> sprites;

      // TODO :: This needs reviewing
      inline ~SpriteGroup(){
        // UPdATE PSRIGORUP!?
        this->pendingErase = true;
        // for(int i = 0; i < this->sprites.size(); i++){
        //   delete sprites[i];
        // }
        this->sprites.clear();

        // std::map<void*, MTR::RND::Image*>::iterator it;
        // for(it = spritePTRs.begin(); it != spritePTRs.end(); it++){
        //     delete it->second;
        // }
        this->spritePTRs.clear();
        // this->update(this);
      }
      

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
      // void placeSprite(void* ptr, MTR::RND::Image* spr);
      
      void removeSprite (MTR::RND::Image* spr);
      void removeSprites(std::vector<MTR::RND::Image*> spr);


      std::map<void*, MTR::RND::Image*> spritePTRs;
    };
  }
}
      
#endif