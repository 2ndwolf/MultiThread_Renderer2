#ifndef _FK_RENDERABLES_h_
#define _FK_RENDERABLES_h_

#include <atomic>
#include <memory>
#include <string>

#include <SDL.h>

// #include "primitives.h"
// #include "fkore.h"
#include "renderable.h"
#include "information.h"

namespace FK{
  namespace AT{
    class Sprite : public ORE::Renderable {
      public:
      // Sprite(Rendering::SpriteInformation* info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);
      Sprite(std::shared_ptr<SpriteInformation> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);


      // JEN AI TU BESOIN
      // void setPosition(WorldPos position);
      // void moveFromPosition(Vec2 move);
      // Internal, do not use
      // void updateSDLRect(); // Is this even required?

      void changeSurface(std::string fileName);

      private:
      static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);

    };



    class Text : public ORE::Renderable{
      public:
      Text(std::shared_ptr<TextInformation> txtInformation, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

      // static std::shared_ptr<Text> loadText(std::string window, int layer, std::shared_ptr<TextInformation> txtInformation, bool ignoreCamera);
      static FKORE::SDLFont* loadFont(std::string fileName, int size);

      void updateText(std::string text);
      void changeFont(std::string fontName);
      void resize(int size);
      // void setColor( Uint8 red, Uint8 green, Uint8 blue ){
      //   color = {red,green,blue};
      // };

      //Set blending??
      // void setBlendMode( SDL_BlendMode blending );
      //Set alpha??
      // void setAlpha( Uint8 alpha );

      SDL_Color color;


      // TTF_Font* gFont;

      // SDL_Surface* sur;

      // RenderTypes type = TEXT;

      private:
      // ID txtID;
      static FKORE::SDLSurface* loadSurface(std::shared_ptr<TextInformation> txtInformation);
      std::shared_ptr<TextInformation> textInformation;

    };
  }
  // END AT namespace

}

#endif