#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

#include "rendering.h"
#include "identifier.h"


SDLA::Rendering::Text::Text(SDLA::Rendering::TextInfo* txtInfo, int layer, bool ignoreCamera, std::string window)
: Renderable(txtInfo->info, layer, window){
  txtID = SDLA::Identifier::newtxtID();

  SDLA::Rendering::SDLSurface* txtSur = SDLA::Rendering::Text::loadSurface(txtInfo->textureText, txtInfo);
  this->textInfo = std::shared_ptr<TextInfo>(txtInfo);
  this->info->fileName = txtSur->fileName;

  srcRect.x = info->area.pos.x;
  srcRect.y = info->area.pos.y;


  if(txtSur->sur != NULL){
    srcRect.w = txtSur->sur->w;
    srcRect.h = txtSur->sur->h;
    sdlRect.w = txtSur->sur->w;
    sdlRect.h = txtSur->sur->h;
  }

  sdlRect.x = 0;
  sdlRect.y = 0;
}

std::shared_ptr<SDLA::Rendering::Text> SDLA::Rendering::Text::loadText(std::string window, int layer, SDLA::Rendering::TextInfo* txtInfo, bool ignoreCamera){

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  std::shared_ptr<Text> txt = std::make_shared<Text>(txtInfo, layer, ignoreCamera, window);
  txt->ownerGroup = sG;
  sG->sprites.push_back(txt);
  sG->ignoreCamera = ignoreCamera;
  sG->info = std::make_shared<SpriteInfo>();
  // mutex.lock();
  windows[window]->getLayer(layer)->groups.push_back(sG);
  // mutex.unlock();
  return txt;
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

// void SDLA::Rendering::Text::updateText(std::string text){
//   this->info->
// }

SDLA::Rendering::SDLSurface* SDLA::Rendering::Text::loadSurface(std::string textureText, SDLA::Rendering::TextInfo* txtInfo){

  SDLSurface newSur;
  newSur.sur = TTF_RenderText_Solid(loadFont(txtInfo->fontName, txtInfo->size)->font, textureText.c_str(), txtInfo->textColor);
  newSur.fileName = "TEXT" + std::to_string(SDLA::Identifier::newtxtID()) + txtInfo->fontName + std::to_string(txtInfo->size);
  surfaces.insert({newSur.fileName, newSur});

  return &surfaces[newSur.fileName];
}

SDLA::Rendering::SDLFont* SDLA::Rendering::Text::loadFont(std::string fileName, int size){
  std::string fontName = fileName + std::to_string(size);
  if(!fonts.count(fontName)){
    Rendering::SDLFont newFont;
    newFont.font = TTF_OpenFont(fileName.c_str(), size);
    newFont.fileName = fontName;
    fonts.insert({fontName, newFont});
  }

  return &fonts[fontName];
}