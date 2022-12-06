#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

// #include "rendering.h"
#include "renderable.h"
#include "renderables.h"
#include "information.h"
#include "identifier.h"
#include "fkore.h"
// SDL_GetWindowFlags()
namespace FK{
  namespace AT{

    Text::Text(std::shared_ptr<TextInformation> txtInfo, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup)
    : ORE::Renderable(std::make_shared<SpriteInformation>(txtInfo), layer, window, ownerGroup){
      // txtID = FK::ORE::Identifier::newtxtID();

      FKORE::SDLSurface* txtSur = Text::loadSurface(txtInfo);
      this->sprInfoWrite->fileName = txtSur->fileName;
      this->textInformation = txtInfo;
      RefreshSpriteInfo();

      srcRect.x = sprInfoRead->area.pos.x;
      srcRect.y = sprInfoRead->area.pos.y;


      if(txtSur->sur != NULL){
        srcRect.w = txtSur->sur->w;
        srcRect.h = txtSur->sur->h;
        tgtRect.w = txtSur->sur->w;
        tgtRect.h = txtSur->sur->h;
      }

      tgtRect.x = 0;
      tgtRect.y = 0;
    }



    // void SDLA::Rendering::Text::updateText(std::string text){
    //   this->info->
    // }

    FKORE::SDLSurface* Text::loadSurface(std::shared_ptr<TextInformation> txtInfo){

      FKORE::SDLSurface newSur;
      newSur.sur = TTF_RenderText_Solid(loadFont(txtInfo->fontName, txtInfo->size)->font, txtInfo->text.c_str(), txtInfo->textColor);
      if(newSur.sur == NULL) newSur.sur = IMG_Load(placeHolder.c_str());
      newSur.fileName = "TEXT" + std::to_string(FK::ORE::Identifier::newtxtID()) + txtInfo->fontName + std::to_string(txtInfo->size);
      FKORE::surfaces.insert({newSur.fileName, newSur});

      return &FKORE::surfaces[newSur.fileName];
    }

    FKORE::SDLFont* Text::loadFont(std::string fileName, int size){
      std::string fontName = fileName + std::to_string(size);
      if(!FKORE::fonts.count(fontName)){
        FKORE::SDLFont newFont;
        newFont.font = TTF_OpenFont(fileName.c_str(), size);
        newFont.fileName = fontName;
        FKORE::fonts.insert({fontName, newFont});
      }

      return &FKORE::fonts[fontName];
    }
  }
}
