#ifndef _MTR_IMAGE_h_
#define _MTR_IMAGE_h_

#include <SDL.h>

#include "renderable.h"
#include "surfaces.h"
// #include "window.h"
#include "defaults.h"

namespace MTR{
  namespace RND{
    class SpriteGroup;

    class Image : public Renderable {
      public:
      std::string fileName = Defaults::placeHolder;

      // String is the window owner's name
      std::map<std::string, SDL_Texture*> textures;

      Bounds   area   = {0,0,0,0};
      int      layer  = 0;
      SDL_Rect tgtRect;
      SDL_Rect srcRect;

      Image(std::vector<std::string> pwindows):
      Renderable(pwindows)
      {};

      // overloaded by Text - sets the image or sheet used by the entity
      void setSurface(const std::string& fileName, bool fromSpriteSheet = false);

      // static void update(Image* image);

      // Group a set of ``Image``s with a ``SpriteGroup``
      static void group(SpriteGroup* sG, std::vector<Image*> imgs);
      // Group a single ``Image`` with a given ``SpriteGroup``
      void group(SpriteGroup* sG);

      // Copy constructor (operator=) cuz I'm a noob
      static void deepCopy(Image* source, Image* target);

      // Set the area to display from the loaded surface in "setSurface"
      void setCrop(Bounds crop);





      // Verify if this function is required and implement it if so
      // void switchLayer(int newLayer){
      //   if(newLayer != layer){
      //     la
      //   }
      // }


      // ~Image(){;;;
            //   // delete texture; <---- do this in display loop so it doesn't DIE
      // }

    };

  }

}

#endif