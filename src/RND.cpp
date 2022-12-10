#include <SDL.h>

#include "defaults.h"
#include "identifier.h"
#include "primitives.h"

#include "text.h"
#include "image.h"
#include "superGroup.h"
#include "spriteGroup.h"
#include "window.h"
#include "surfaces.h"

namespace MTR{
  namespace RND{

    void loadSurface(std::string fileName, bool keepImgInMemory = false){
      if(!SUR::surfaces.count(fileName)){
        SUR::SDLSurface newSur;
        newSur.sur = IMG_Load(fileName.c_str());
        if(newSur.sur == NULL) newSur.sur = IMG_Load(Defaults::placeHolder.c_str());
        newSur.fileName = fileName;
        if(keepImgInMemory) newSur.useCount = -1;
        else newSur.useCount = 1;
        SUR::surfaces.insert({fileName, newSur});

      } else if (keepImgInMemory){
        SUR::surfaces[fileName].useCount = -1;

      } else if (SUR::surfaces[fileName].useCount != -1){
        SUR::surfaces[fileName].useCount += 1;
      }

    }

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


    void Text::setText (std::string newText){setSurface(newText);};

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

    void Layer::update(Layer layer){
      if(layer.windows.size() != 0){
        for(int i = 0; i < layer.windows.size(); i++){
          Window::getWindow(layer.windows[i])->
          buffer.writeBuffer.updLayer->upd[layer.layerNumber] = layer;
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
      
    // TODO :: FIGURE OUT SOMETHING! (this looks unprefeesional!)
    bool Renderable::checkSanity(){
      return (
        !(bounds == (Bounds){{INT_MIN, INT_MIN}, {0, 0}}) &&
        !(windows.size() == 0) &&
        !(ownerGroup == nullptr)
        // rnd->rotCenter != 
      );
    }

    bool SuperGroup::checkSanity(){
      return (
        !(bounds == (Bounds){{INT_MIN, INT_MIN}, {0, 0}})
        // !(windows.size() == 0) &&
        // !(ownerGroup == nullptr)
        // rnd->rotCenter != 
      );
    }

    bool SpriteGroup::checkSanity(){
      return (
        !(bounds == (Bounds){{INT_MIN, INT_MIN}, {0, 0}}) &&
        !(windows.size() == 0) 
        // !(ownerGroup == nullptr)
        // rnd->rotCenter != 
      );
    }

    void Renderable::group(SuperGroup* supG){
      supG->refreshBounds(this);
      refreshRotCenter();
      ownerGroup = supG;
    }

    bool Renderable::addToQueue(){
      if(!isQueued){
        isQueued = true;
        return true;
      }
      return false;
    }

    void Renderable::refreshRotCenter(){
      if(angleFrSuper){
        rotCenter.x =
        ownerGroup->bounds.box.width / 2;
        rotCenter.y =
        ownerGroup->bounds.box.height / 2;
      }
    }

    void Image::group(SpriteGroup* sG, std::vector<Image*> imgs){
      for(int i = 0; i < imgs.size(); i++){
        imgs[i]->Renderable::group(sG);
      }
      sG->placeSprites(imgs);
    }

    void Image::group(SpriteGroup* sG){
      this->Renderable::group(sG);
      sG->placeSprite(this);
    }

    void SpriteGroup::refreshBounds(Renderable* rend){
      Vec2 oldPos = bounds.pos;
      SuperGroup::refreshBounds(rend);

      if(bounds.pos != oldPos && !ignoreCamera){
        worldPos.x -= bounds.pos.x - oldPos.x;
        worldPos.y -= bounds.pos.y - oldPos.y;
      }
    }
    void SpriteGroup::placeSprites(std::vector<MTR::RND::Image*> spr){
      for(int i = 0; i < spr.size(); i++){
        placeSprite(spr[i]);
      }
    }

    void SpriteGroup::placeSprite(MTR::RND::Image* spr){
      sprites[&spr] = spr;
    }
    
    void SpriteGroup::placeSprite(void* ptr, MTR::RND::Image* spr){
      sprites.emplace(ptr, spr);
    }    

    void SuperGroup::refreshBounds(Renderable* rend){
      // Vec2 minOffset = {INT_MAX,INT_MAX};
      // Box groupBox = {0,0};
      bounds.box.width  = 
      std::max(rend->bounds.box.width , bounds.box.width  + bounds.pos.x);
      bounds.box.height = 
      std::max(rend->bounds.box.height, bounds.box.height + bounds.pos.y);

      Vec2 oldPos = bounds.pos;
      // TODO :: WorldPos?
      bounds.pos.x = 
      std::min(rend->bounds.pos.x, bounds.pos.x);
      bounds.pos.y = 
      std::min(rend->bounds.pos.y, bounds.pos.y);
    }

    void Renderable::deepCopy(Renderable* source, Renderable* target){
      target->bounds       = source->bounds       ;
      target->windows      = source->windows      ;
      target->hidden       = source->hidden       ;
      target->angle        = source->angle        ;
      target->rotCenter    = source->rotCenter    ;
      target->zoomCenter   = source->zoomCenter   ;
      target->zoom         = source->zoom         ;
      target->flip         = source->flip         ;
      target->angleFrSuper = source->angleFrSuper ;
      target->ownerGroup   = source->ownerGroup   ;
    }

    void Image::deepCopy(Image* source, Image* target){
        Renderable::deepCopy(source, target);

        target->area     = source->area;
        target->layer    = source->layer;
        target->tgtRect  = source->tgtRect;
        target->srcRect  = source->srcRect;
        target->fileName = source->fileName;
    }


    void SpriteGroup::deepCopy(SpriteGroup* source, SpriteGroup* target){
      SuperGroup::deepCopy(source, target);

      target->layer        = source->layer;
      target->ignoreCamera = source->ignoreCamera;
      target->worldPos     = source->worldPos;
    }


    void SuperGroup::deepCopy(SuperGroup* source, SuperGroup* target){
        Renderable::deepCopy(source, target);
    }

    void Text::deepCopy(Text* source, Text* target){
        Image::deepCopy(source, target);

        target->font      = source->font;
        target->size      = source->size;
        target->textColor = source->textColor;
    }


  }


}

