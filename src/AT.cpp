#include <memory>
#include <string>
#include <vector>

#include "window.h"
#include "renderables.h"
#include "information.h"
#include "AT.h"

namespace FK{
  namespace AT{

    std::shared_ptr<SpriteGroup> addSprite(std::string window, int layer, std::shared_ptr<SpriteInformation> info, bool ignoreCamera){
      std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>(std::make_shared<FK::AT::Information>(window));
      std::shared_ptr<Sprite>       s = std::make_shared<Sprite     >(info, layer, ignoreCamera, window, sG);
      // s->ownerGroup = sG;
      sG->sprites.push_back(s);
      sG->ignoreCamera = ignoreCamera;
      // sG->info = std::make_shared<SpriteInfo>();
      FK::Window::getWindow(window)->getLayer(layer)->groups.push_back(sG);
        // SDLA::Rendering::TextInfo* textInfo = new SDLA::Rendering::TextInfo();
        // textInfo->info = new SDLA::Rendering::SpriteInfo();
        // textInfo->info->offset = info->offset;
        // textInfo->textureText = std::to_string(windows[window]->getWriteBuffer()[0]->id) + " " + std::to_string(windows[window]->getWriteBuffer()[1]->id) ;
        // textInfo->fontName = "assets/Minecraft.ttf";
        // textInfo->size = 12;
        // textInfo->textColor = (SDL_Color) {255,255,255};
        // SDLA::Rendering::Text::loadText(window, 1, textInfo, true);
      return sG;
    }

    // DO NOT Add an overload for std::shared_ptr<SpriteGroup> where it adds the images to the group instead of creating a new group
    // It won't be possible to add sprites to a spriteGroup -> segmentation fault possibility
    // std::vector<std::shared_ptr<SDLA::Rendering::Sprite>> Rendering::Sprite::addImageGroup(std::string window, int layer, SpriteGroup* groupInfo, std::vector<SpriteInfo*> group){

    //   std::shared_ptr<SpriteGroup> sG = std::shared_ptr<SpriteGroup>(groupInfo);
    //   sG->sprites = {};
    //   std::vector<std::shared_ptr<Sprite>> sV;
    //   for(int i = 0; i < group.size(); i++){
    //     std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i], layer, sG->ignoreCamera, window, sG);
    //     sV.push_back(s);
    //     sG->sprites.push_back(s);
    //   }

    //   windows[window]->getLayer(layer)->groups.push_back(sG);
    //   return sV;
    // }

    // DO NOT Add an overload for std::shared_ptr<SpriteGroup> where it adds the images to the group instead of creating a new group
    // It won't be possible to add sprites to a spriteGroup -> segmentation fault possibility
     std::shared_ptr<SpriteGroup> addSpriteGroup(std::string window, int layer, std::shared_ptr<Information> groupInfo, std::vector<std::shared_ptr<SpriteInformation>> group){

      std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>(groupInfo);
      // delete groupInfo;
      // sG->sprites = {};
      std::vector<std::shared_ptr<Sprite>> sV;
      for(int i = 0; i < group.size(); i++){
        std::shared_ptr<Sprite> s = std::make_shared<Sprite>(group[i], layer, sG->ignoreCamera, window, sG);
        sV.push_back(s);
        // SDL_Rect rect = s->getSDLRect();
        // std::shared_ptr<SpriteInformation> sprInfo = s->information;
        // sG->bounds = sG->bounds + sprInfo->bounds;
        sG->sprites.push_back(s);
      }

      Window::getWindow(window)->getLayer(layer)->groups.push_back(sG);
      return sG;
    }

    std::shared_ptr<SpriteGroup> loadText(std::string window, int layer, std::shared_ptr<TextInformation> txtInfo, bool ignoreCamera){

      std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>(std::make_shared<Information>(window));
      std::shared_ptr<Text> txt = std::make_shared<Text>(txtInfo, layer, ignoreCamera, window, sG);
      // txt->ownerGroup = sG;
      // sG->sprites = {};
      sG->sprites.push_back(txt);
      sG->ignoreCamera = ignoreCamera;
      // sG->info = std::make_shared<SpriteInfo>();
      Window::getWindow(window)->mutex.lock();
      Window::getWindow(window)->getLayer(layer)->groups.push_back(sG);
      Window::getWindow(window)->mutex.unlock();
      return sG;
      //Get rid of preexisting texture
      // free();


      //Render text surface



      // Rendering::surfaces[] 
      // return true;
      // if( textSurface == NULL )
      // {
      //     printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
      // }
      // else
      // {
      //     //Create texture from surface pixels
      //     mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
      //     if( mTexture == NULL )
      //     {
      //         printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
      //     }
      //     else
      //     {
      //         //Get image dimensions
      //         mWidth = textSurface->w;
      //         mHeight = textSurface->h;
      //     }

      //     //Get rid of old surface
      //     SDL_FreeSurface( textSurface );
      // }
      
      // //Return success
      // return mTexture != NULL;
    }
    // }
  }
}