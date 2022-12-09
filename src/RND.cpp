// #include "rendering.h"
#include <SDL.h>

#include "defaults.h"
#include "identifier.h"
#include "primitives.h"

#include "text.h"
#include "image.h"
#include "superGroup.h"
#include "spriteGroup.h"

namespace MTR{
  namespace RND{

    void Image::setSurface(std::string fileName){
      if(SUR::surfaces[this->fileName].useCount != -1){
        SUR::surfaces[this->fileName].useCount -= 1;
        if(SUR::surfaces[this->fileName].useCount == 0) SUR::surfaces.erase(this->fileName);
      }

      this->fileName = fileName;
      SUR::loadSurface(fileName);

      for(int i = 0; i < windows.size(); i++){
        textures[windows[i]] = SDL_CreateTextureFromSurface(
          Window::getWindow(windows[i])->getContext(), SUR::surfaces[fileName].sur);
      }

      // TODO :: std::map<std::string, Bounds> areaS;
      // textures[windows[0]]->
      SDL_QueryTexture(textures[windows[0]], NULL, NULL, &area.box.width, &area.box.height);
    };

    void Text::setSurface(std::string newText){
      if(SUR::surfaces.count(fileName)) SUR::surfaces.erase(fileName);

      SDL_Color color;
      color.r = textColor.r;
      color.g = textColor.g;
      color.b = textColor.b;
      color.a = textColor.a;

      SUR::SDLSurface newSur;

      newSur.sur = TTF_RenderText_Solid(font.font, newText.c_str(), color);
      if(newSur.sur == NULL) newSur.sur = IMG_Load(Defaults::placeHolder.c_str());

      newSur.fileName = "TEXT" + std::to_string(Identifier::newtextID()) + font.fileName;

      fileName = newSur.fileName;

      SUR::surfaces.insert({newSur.fileName, newSur});
      for(int i = 0; i < windows.size(); i++){
        textures[windows[i]] = SDL_CreateTextureFromSurface(
          Window::getWindow(windows[i])->getContext(), SUR::surfaces[newSur.fileName].sur);
      }

      // TODO :: std::map<std::string, Bounds> areaS;
      SDL_QueryTexture(textures[windows[0]], NULL, NULL, &area.box.width, &area.box.height);
    }

    SUR::Font Text::loadFont(std::string fileName, int size){
      std::string fontName = fileName + std::to_string(size);

      if(!SUR::fonts.count(fontName)){
        SUR::Font newFont;
        newFont.font = TTF_OpenFont(fileName.c_str(), size);
        newFont.fileName = fontName;
        SUR::fonts.insert({fontName, newFont});
      }

      // TODO :: GARBAGE COLLECTIOMN IN THSOT THING!
      return SUR::fonts[fontName];
    }

    // UPDATE OVERLOADS
    void Image::update(Image* image){
      if(image->checkSanity()){
        for(int i = 0; i < image->windows.size(); i++){
          Window* win = Window::getWindow(image->windows[i]);

          win->buffer.writeBuffer.updSprite->upd
          [win->clampLayerIndex(image->layer)]
          .emplace((void*)&image, *image);
        }
      }
    }

    void SpriteGroup::update(SpriteGroup* group){
      if(group->checkSanity()){
        for(int i = 0; i < group->windows.size(); i++){
          Window* win = Window::getWindow(group->windows[i]);

          win->buffer.writeBuffer.updSpriteGroup->upd
          [win->clampLayerIndex(group->layer)]
          .emplace((void*)&group, *group);
        }
      }
    }

    void SuperGroup::update(SuperGroup* group){
      if(group->checkSanity()){
        for(int i = 0; i < group->windows.size(); i++){
          Window::getWindow(group->windows[i])->
          buffer.writeBuffer.updSuperGroup->upd
          .emplace((void*)&group, *group);
        }
      }
    }

    void Text::update(Text* image){
      if(image->checkSanity()){
        for(int i = 0; i < image->windows.size(); i++){
          Window* win = Window::getWindow(image->windows[i]);

          win->buffer.writeBuffer.updSpriteGroup->upd
          [win->clampLayerIndex(image->layer)]
          .emplace((void*)&image, *image);
        }
      }
    }

    // MANIPULATION
    void Image::setCrop(Bounds crop){
        area = crop;
        srcRect.w = crop.box.width;
        srcRect.h = crop.box.height;
        srcRect.x = crop.pos.x;
        srcRect.y = crop.pos.y;
        tgtRect.w = crop.box.width;
        tgtRect.h = crop.box.height;
      }
  }
}

