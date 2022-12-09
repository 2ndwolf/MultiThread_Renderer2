#ifndef _MTR_IMAGE_h_
#define _MTR_IMAGE_h_

#include <SDL.h>

#include "renderable.h"
#include "surfaces.h"
#include "window.h"
#include "defaults.h"

namespace MTR{
  // inline std::string placeHolderImage = "assets/gen_specialchest.png";
  namespace RND{

    class Image : public Renderable {
      public:
      // overloaded by Text
      void setSurface(std::string fileName);
      static void update(Image* image);

      static void group(SpriteGroup* sG, std::vector<Image*> imgs){
        for(int i = 0; i < imgs.size(); i++){
          imgs[i]->Renderable::group(sG);
        }
        sG->placeSprites(imgs);
      }

      void group(SpriteGroup* sG){
        this->Renderable::group(sG);
        sG->placeSprite(this);
      }

      // void switchLayer(int newLayer){
      //   if(newLayer != layer){
      //     la
      //   }
      // }

      Bounds area = {0,0,0,0};
      int layer = 0;


      SDL_Rect tgtRect;
      SDL_Rect srcRect;
      std::string fileName = Defaults::placeHolder;

      void setCrop(Bounds crop);

      // ~Image(){
      //   // delete texture; <---- do this in display loop so it doesn't DIE
      // }

      std::map<std::string, SDL_Texture*> textures;
      // protected:
    };

  }

}

#endif