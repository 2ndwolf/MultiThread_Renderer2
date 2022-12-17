#ifndef _MTR_IMAGE_h_
#define _MTR_IMAGE_h_

#include <utility>

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
      bool surfaceUpdated = true;
      bool surfaceFrSpriteSheet = false;
      SpriteGroup* ownerGroupObj = nullptr;

      // String is the window owner's name
      SDL_Texture* texture;
      // std::map<void*, SDL_Texture*> dfrTex 

      Bounds area = {0,0,0,0};
      int      layer  = 0;
      SDL_Rect tgtRect;
      SDL_Rect srcRect;

      Image(std::vector<std::string> pwindows):
      Renderable(pwindows)
      {};

      // Image(){};

      Image(const Image& old_obj) : Renderable(old_obj){
        area     = old_obj.area    ;
        layer    = old_obj.layer   ;
        tgtRect  = old_obj.tgtRect ;
        srcRect  = old_obj.srcRect ;
        fileName = old_obj.fileName;
        surfaceUpdated = old_obj.surfaceUpdated;
        surfaceFrSpriteSheet = old_obj.surfaceFrSpriteSheet;
      }

      // Image(Image&& other) : Renderable((Renderable)other){
      //   area     = other.area    ;
      //   layer    = other.layer   ;
      //   tgtRect  = other.tgtRect ;
      //   srcRect  = other.srcRect ;
      //   fileName = other.fileName;
      //   // texture  = other.texture ;

      //   other.area     = {0,0,0,0};
      //   other.layer    = 0        ;
      //   other.tgtRect  = {0,0,0,0};
      //   other.srcRect  = {0,0,0,0};
      //   other.fileName = ""       ;
      //   other.texture  = nullptr  ;
      // }

      Image& operator=(const Image& other){

        if(this!=&other){
          Image(other).swap(*this);
          // return tmp;
        }
        return *this;
      }

      void swap(Image& i){
        std::swap(area    , i.area    );
        std::swap(layer   , i.layer   );
        std::swap(tgtRect , i.tgtRect );
        std::swap(srcRect , i.srcRect );
        std::swap(fileName, i.fileName);
        std::swap(surfaceUpdated, i.surfaceUpdated);
        std::swap(surfaceFrSpriteSheet, i.surfaceFrSpriteSheet);


      }

      Image& operator=(Image&& other){
        if(this != &other){
          Renderable::operator=(other);
          area     = other.area    ;
          layer    = other.layer   ;
          tgtRect  = other.tgtRect ;
          srcRect  = other.srcRect ;
          fileName = other.fileName;
          surfaceUpdated = other.surfaceUpdated;
          surfaceFrSpriteSheet = other.surfaceFrSpriteSheet;

          // texture  = other.texture ;

          other.area     = {0,0,0,0};
          other.layer    = 0        ;
          other.tgtRect  = {0,0,0,0};
          other.srcRect  = {0,0,0,0};
          other.fileName = ""       ;
          other.surfaceUpdated = false;
          surfaceFrSpriteSheet = false;
          // other.texture  = nullptr  ;
        }
        return *this;
      }

      // overloaded by Text - sets the image or sheet used by the entity
      void setSurface(const std::string& fileName, bool fromSpriteSheet = false);

      // static void update(Image* image);

      // Group a set of ``Image``s with a ``SpriteGroup``
      static void group(SpriteGroup* sG, std::vector<Image*> imgs);
      // Group a single ``Image`` with a given ``SpriteGroup``
      // void subscribeTo(SpriteGroup* sG);

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