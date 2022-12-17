#ifndef _MTR_SPRITEGROUP_h_
#define _MTR_SPRITEGROUP_h_

#include <vector>
#include <atomic>
#include <utility>

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

      // static void deepCopy(SpriteGroup* source, SpriteGroup* target);

      SpriteGroup(std::vector<std::string> pwindows):
        SuperGroup(pwindows)
      {
        // this->windows = pwindows;
      };

      // SpriteGroup(){};

      // Copy constructor
      SpriteGroup(const SpriteGroup& old_obj) : SuperGroup(old_obj){
        layer        = old_obj.layer       ;
        ignoreCamera = old_obj.ignoreCamera;
        worldPos     = old_obj.worldPos    ;
        spriteDfr    = old_obj.spriteDfr   ;
      }

      SpriteGroup(SpriteGroup&& other) : SuperGroup(other){
        layer        = other.layer        ;
        ignoreCamera = other.ignoreCamera ;
        worldPos     = other.worldPos     ;
        spriteDfr    = other.spriteDfr    ;

        other.layer        = 0;
        other.ignoreCamera = false;
        other.worldPos     = {0,0};
        other.spriteDfr    .clear();

      }

      // SpriteGroup(std::map<void*, MTR::RND::Image> sprites){
      //   updateSprites(sprites);
      // };
      std::vector<MTR::RND::Image*> sprites;

      // TODO :: This needs reviewing
      ~SpriteGroup(){
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
        // this->spritedfr.clear();
        // this->update(this);
      }

      SpriteGroup& operator=(const SpriteGroup& other){
        if(this!=&other){
          SpriteGroup(other).swap(*this);
        }
        return *this;
      }

      void swap(SpriteGroup& sG){
        std::swap(layer       , sG.layer        );
        std::swap(ignoreCamera, sG.ignoreCamera );
        std::swap(worldPos    , sG.worldPos     );
        std::swap(spriteDfr   , sG.spriteDfr    );
      }

      SpriteGroup& operator=(SpriteGroup&& other){
        if(this != &other){
          SuperGroup::operator=(other);
          layer        = other.layer        ;
          ignoreCamera = other.ignoreCamera ;
          worldPos     = other.worldPos     ;
          spriteDfr    = other.spriteDfr    ;

          other.layer        = 0;
          other.ignoreCamera = false;
          other.worldPos     = {0,0};
          other.spriteDfr    .clear();          // SpriteGroup tmp(std::move(other));
          // std::swap(*this,tmp);
        }
        return *this;
      }

      static void update(std::vector<SpriteGroup*> groups);

      // inline void updateSprites(std::map<void*, FK::AT::Image> sprites){
      //   std::map<void*, FK::AT::Image>::iterator it;
      //   for(it = sprites.begin(); it != sprites.end(); it++){
      //     this->sprites.emplace(it->first, &it->second);
      //   }
      //   // setUpdate();
      // }

      bool checkSanity();
      void refreshBounds();

      void placeSprites(std::vector<MTR::RND::Image*> spr);
      void placeSprite(MTR::RND::Image* spr);
      // void placeSprite(void* ptr, MTR::RND::Image* spr);
      
      void removeSprite (MTR::RND::Image* spr);
      void removeSprites(std::vector<MTR::RND::Image*> spr);


      std::map<void*, MTR::RND::Image> spriteDfr;
    };
  }
}
      
#endif