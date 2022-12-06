#ifndef _FK_RENDERABLE_h_
#define _FK_RENDERABLE_h_
/*
#include <atomic>
#include <memory>
#include <string>

#include <SDL.h>

#include "TSRPrimitives.h"
#include "fkore.h"
#include "information.h"
#include "window.h"
#include "fkenum.h"

namespace FK{
  namespace ORE{
    inline std::string placeHolderImage = "assets/gen_specialchest.png";
    class Renderable {
      protected:
      bool getAngleFromGroup = true;
      

      public:

      std::shared_ptr<FK::AT::SpriteInformation> information;
      void changeSurface(std::string fileName);
      void setRotationCenter(Vec2 center);

      // inline FK::AT::SpriteInformation getInformation(){return *information;};
      // bool hidden = false;
      bool pendingErase = false;
      std::string myWindow;
      std::shared_ptr<FK::AT::SpriteGroup> ownerGroup;

      SDL_Texture* const getTexture(){return texture;};
      void setCrop(Bounds crop, bool rendererCall = false);
      bool getIgnoreCamera(){return ownerGroup->ignoreCamera;}

      Bounds getBounds();

      // information->flip = ENUM::Flip
      void setFlip(ENUM::Flip flip);


      // For display loop exclusively
      void const setTexture(SDL_Texture* tex){texture = tex;};

      // TODO :: WAS MOVED AROUND
      // static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);
      static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);



      // RenderTypes type;

      SDL_Rect* const getSRCRect(){return &srcRect;};
      SDL_Rect const getSDLRect(){return tgtRect;};
      std::string const getFileName(){return information->fileName;};
      std::atomic<bool> texQueued = true;

      protected:

      Renderable(std::shared_ptr<FK::AT::SpriteInformation> information, int layer, std::string window, std::shared_ptr<FK::AT::SpriteGroup> ownerGroup){
        // this->ignoreCamera = ignoreCamera;
        // std::shared_ptr<SpriteInfo> sprI = std::make
        this->information = information;
        this->layer = FK::Window::getWindow(window)->clampLayerIndex(layer);
        // this->info->layer = this->layer;
        this->myWindow = window;
        this->ownerGroup = ownerGroup;
        // this->fileName = info->fileName;
      };

      SDL_Rect tgtRect;
      SDL_Rect srcRect;
      // std::string fileName;
      // std::atomic<bool> ignoreCamera = true;
      int layer;
      inline static std::string placeHolder = placeHolderImage;


      private:

      private:
      SDL_Texture* texture;

    };
  }

  // namespace AT{
  //   class Sprite : public ORE::Renderable {
  //     public:
  //     // Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);
  //     Sprite(std::shared_ptr<SpriteInfo> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

  //     static std::shared_ptr<Sprite> addImage(std::string window, int layer, std::shared_ptr<SpriteInfo> info, bool ignoreCamera);
  //     // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
  //     // static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, std::shared_ptr<Info> groupInfo, std::vector<SpriteInfo*> group);
  //     static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, std::shared_ptr<Info> groupInfo, std::vector<std::shared_ptr<SpriteInfo>> group);
  //     static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);

  //     // JEN AI TU BESOIN
  //     // void setPosition(WorldPos position);
  //     // void moveFromPosition(Vec2 move);
  //     // Internal, do not use
  //     // void updateSDLRect(); // Is this even required?

  //     void setFlip();

  //     void changeSurface(std::string fileName);

  //   };



  //   class Text : public ORE::Renderable{
  //     public:
  //     Text(std::shared_ptr<TextInfo> txtInfo, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

  //     static std::shared_ptr<Text> loadText(std::string window, int layer, std::shared_ptr<TextInfo> txtInfo, bool ignoreCamera);
  //     static FKORE::SDLFont* loadFont(std::string fileName, int size);
  //     static FKORE::SDLSurface* loadSurface(std::shared_ptr<TextInfo> txtInfo);

  //     void updateText(std::string text);
  //     void changeFont(std::string fontName);
  //     void resize(int size);
  //     // void setColor( Uint8 red, Uint8 green, Uint8 blue ){
  //     //   color = {red,green,blue};
  //     // };

  //     //Set blending??
  //     // void setBlendMode( SDL_BlendMode blending );
  //     //Set alpha??
  //     // void setAlpha( Uint8 alpha );

  //     SDL_Color color;


  //     // TTF_Font* gFont;

  //     // SDL_Surface* sur;

  //     // RenderTypes type = TEXT;

  //     private:
  //     ID txtID;

  //     std::shared_ptr<TextInfo> textInfo;

  //   };
  // }
  // END AT namespace

}
*/
#endif