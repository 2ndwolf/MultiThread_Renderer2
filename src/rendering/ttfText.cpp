#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

#include "rendering.h"
#include "identifier.h"


SDLA::Rendering::Text::Text(SDLA::Rendering::SpriteInfo* info, std::string textureText, std::string fontName, int size, SDL_Color textColor){

  SDLA::Rendering::SDLSurface* txtSur = SDLA::Rendering::loadTextSurface(textureText, loadFont(fontName,size)->font, textColor);
  this->info = info;
  this->info->fileName = txtSur->fileName;
  myWin = win;

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

  color = textColor;
}

std::shared_ptr<SDLA::Rendering::SpriteGroup> SDLA::Rendering::Text::loadText(std::shared_ptr<SDLA::Rendering::Window> window, int layer, SDLA::Rendering::SpriteInfo* info, std::string textureText, std::string fontName, int size, SDL_Color textColor){
  setWorkingWindow(window);

  if(layer > win->getLayerCount()) layer =  win->getLayerCount();
  else if(layer < 0) layer = 0;

  std::shared_ptr<SpriteGroup> sG = std::make_shared<SpriteGroup>();
  sG->sprites.push_back(std::make_shared<Text>(info, textureText, fontName, size, textColor));
  sG->info = info;
  mtx.lock();
  win->getLayer(layer)->groupBuffer.push_back(sG);
  mtx.unlock();
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