#include <memory>
#include <string>

#include "rendering.h"
#include "identifier.h"
#include <SDL_ttf.h>

using namespace SDLA;

std::shared_ptr<Rendering::Window> Rendering::newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      Vec2 position,
      SDL_WindowFlags mode
      ){
  std::shared_ptr<Window> window = std::make_shared<Window>(layerCount, windowSize, /* this->threadID,  */name, position, mode);

  windows.members.push_back(window);
  return window;
}

// TODO check if image exists first
Rendering::SDLSurface* Rendering::loadSurface(std::string fileName, bool keepImgInMemory){
  if(!surfaces.count(fileName)){
    Rendering::SDLSurface newSur;
    newSur.sur = IMG_Load(fileName.c_str());
    newSur.fileName = fileName;
    if(keepImgInMemory) newSur.useCount = -1;
    else newSur.useCount = 1;
    surfaces.insert({fileName, newSur});

  } else if (keepImgInMemory){
    surfaces[fileName].useCount = -1;

  } else if (surfaces[fileName].useCount != -1){
    surfaces[fileName].useCount += 1;
  }

  return &surfaces[fileName];
}

Rendering::SDLFont* Rendering::loadFont(std::string fileName, int size){
  std::string fontName = fileName + std::to_string(size);
  if(!fonts.count(fontName)){
    Rendering::SDLFont newFont;
    newFont.font = TTF_OpenFont(fileName.c_str(), size);
    newFont.fileName = fontName;
    fonts.insert({fontName, newFont});
  }

  return &fonts[fontName];
}

SDLA::Rendering::SDLSurface* Rendering::loadTextSurface(std::string textureText, TTF_Font* font, SDL_Color textColor){

  SDLSurface newSur;
  newSur.sur = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
  newSur.fileName = "TEXT" + std::to_string(SDLA::Identifier::newtxtID());
  surfaces.insert({newSur.fileName, newSur});

  return &surfaces[newSur.fileName];
}

