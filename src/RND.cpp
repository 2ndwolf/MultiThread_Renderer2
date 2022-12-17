#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstring>
#include <string>
#include <algorithm>

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
  // namespace SUR{
    void SUR::loadSurface(const std::string& fileName, bool keepImgInMemory){
      if(!SUR::surfaces.count(fileName)){
        SUR::SDLSurface newSur;

        char fileLoc[strlen(Defaults::rootFolder) + strlen(fileName.c_str())];
        std::strcpy(fileLoc, Defaults::rootFolder);
        std::strcat(fileLoc, fileName.c_str());

        newSur.sur = IMG_Load(fileLoc);

        if(newSur.sur == nullptr) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading image", IMG_GetError(), NULL); 
        if(newSur.sur == nullptr) newSur.sur = IMG_Load(Defaults::placeHolder.c_str());
        if(newSur.sur == nullptr) SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error loading placeholder", IMG_GetError(), NULL);

        newSur.fileName = fileName;
        if(keepImgInMemory) newSur.useCount = -1;
        else newSur.useCount = 1;
        SUR::surfaces.emplace(fileName, newSur);

      } else if (keepImgInMemory){
        SUR::surfaces[fileName].useCount = -1;

      } else if (SUR::surfaces[fileName].useCount != -1){
        SUR::surfaces[fileName].useCount += 1;
      }

    }
  // }
  namespace RND{


    void Image::setSurface(const std::string& fileName, bool fromSpriteSheet){
      if(SUR::surfaces[this->fileName].useCount != -1){
        SUR::surfaces[this->fileName].useCount -= 1;
        if(SUR::surfaces[this->fileName].useCount == 0) SUR::surfaces.erase(this->fileName);
      }

      this->fileName = fileName;
      this->surfaceUpdated = true;
      SUR::loadSurface(fileName);

    };


    void Text::setText(const std::string& newText){setSurface(newText);};

    void Text::setSurface(const std::string& newText){
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
      surfaceUpdated = true;
    }

    SUR::Font Text::loadFont(const std::string& fileName, int size){
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

    // UPDATE OVERLOADS (not in need to update, these are literal overloads for the update function!)
    // void Image::update(Image* image){
    //   if(image->checkSanity()){
    //     for(int i = 0; i < image->windows.size(); i++){
    //       Window* win = Window::getWindow(image->windows[i]);

    //       win->buffer.writeBuffer.updSprite
    //       [win->clampLayerIndex(image->layer)]
    //       .emplace((void*)&image, *image);
    //     }
    //   }
    // }

    void SpriteGroup::update(std::vector<SpriteGroup*> groups){
      for(int i = 0; i < groups.size(); i++){
        // if(groups[i]->checkSanity()){

          for(int j = 0; j < groups[i]->windows.size(); j++){
            Window* win = Window::getWindow(groups[i]->windows[j]);

            win->buffer.writeBuffer.ptrSpriteGroup
            [win->clampLayerIndex(groups[i]->layer)]
            .emplace((void*)groups[i]->self, groups[i]);
          }
        }
      // }
    }
    // void SuperGroup::update(std::vector<SpriteGroup*> groups){
    //   for(int i = 0; i < groups.size(); i++){
    //     if(groups[i]->checkSanity()){
    //       SpriteGroup* tgt = new SpriteGroup{groups[i]->windows};
    //       SpriteGroup::deepCopy(groups[i], tgt);

    //       for(int j = 0; j < groups[i]->windows.size(); j++){
    //         Window* win = Window::getWindow(groups[i]->windows[j]);

    //         if(
    //           win->buffer.writeBuffer.updSpriteGroup
    //           [win->clampLayerIndex(groups[i]->layer)].find((void*)&groups[i]) !=
    //           win->buffer.writeBuffer.updSpriteGroup
    //           [win->clampLayerIndex(groups[i]->layer)].end())
    //           { delete win->buffer.writeBuffer.updSpriteGroup
    //             [win->clampLayerIndex(groups[i]->layer)][(void*)&groups[i]];
    //           }

    //         win->buffer.writeBuffer.updSpriteGroup
    //         [win->clampLayerIndex(groups[i]->layer)]
    //         .emplace((void*)&groups[i], tgt);
    //       }
    //     }
    //   }
    // }
 
    void SuperGroup::update(std::vector<SuperGroup*> groups){
      for(int i = 0; i < groups.size(); i++){
        // if(groups[i]->checkSanity()){
          for(int j = 0; j < groups[i]->windows.size(); j++){

            Window* win = Window::getWindow(groups[i]->windows[j]);
            win->buffer.writeBuffer.ptrSuperGroup.emplace((void*)groups[i]->self, groups[i]);
          }
        }
      // }
    }

    

    // void Text::update(Text* image){
    //   if(image->checkSanity()){

    //     for(int i = 0; i < image->windows.size(); i++){
    //       Window* win = Window::getWindow(image->windows[i]);

    //       win->buffer.writeBuffer.updSpriteGroup
    //       [win->clampLayerIndex(image->layer)]
    //       .emplace((void*)&image, *image);
    //     }
    //   }
    // }
      // sprites.erase(std::remove(sprites.begin(), sprites.end(), spr), sprites.end());

    void Layer::update(Layer* layer){
      if(layer->windows.size() != 0){
        for(int i = 0; i < layer->windows.size(); i++){

          Window::getWindow(layer->windows[i])->
          buffer.writeBuffer.ptrLayer.emplace((void*)layer->self, layer);
        }
      }
    }

    // MANIPULATION
    void Image::setCrop(Bounds crop){
      area = crop;

      bounds.box.width  = crop.box.width ;
      bounds.box.height = crop.box.height;

      srcRect.w = crop.box.width;
      srcRect.h = crop.box.height;
      srcRect.x = crop.pos.x;
      srcRect.y = crop.pos.y;

      tgtRect.w = crop.box.width;
      tgtRect.h = crop.box.height;

      surfaceFrSpriteSheet = true;
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

    // void Renderable::subscribeTo(SuperGroup supG){
    //   ownerGroup = &supG;
    //   supG->updateBounds(this);
    //   refreshRotCenter(supG);
    // }

    // void Image::subscribeTo(SpriteGroup* sG){
    //   this->Renderable::subscribeTo(sG);
    //   sG->placeSprite(this);
    // }


    // void SpriteGroup::subscribeTo(SuperGroup* supG){
    //   ownerGroup = supG;

    // }

    bool Renderable::addToQueue(){
      if(!isQueued){
        isQueued = true;
        return true;
      }
      return false;
    }

    void Renderable::refreshRotCenter(SuperGroup* supG){
      if(angleFrSuper){
        rotCenter.x =
        supG->bounds.box.width / 2;
        rotCenter.y =
        supG->bounds.box.height / 2;
      }
    }

    void Image::group(SpriteGroup* sG, std::vector<Image*> imgs){
      for(int i = 0; i < imgs.size(); i++){
        imgs[i]->ownerGroup    = sG->self;
        imgs[i]->ownerGroupObj = sG;
        sG->updateBounds(imgs[i]);
        // imgs[i]->Renderable::subscribeTo(sG);
        imgs[i]->refreshRotCenter(sG);
      }
      sG->placeSprites(imgs);
    }

    int SuperGroup::groupSuper(std::vector<SuperGroup*> spGs, bool setCenter){
      if(!any_of(pool.begin(), pool.end(), [&](const SuperGroup* supG) {return supG == this;}
      )){
        pool.push_back(this);
      }

      for(int i = 0; i < spGs.size(); i++){
        // this->useCount++;

        childs.push_back(spGs[i]);
        spGs[i]->angleFrSuper = setCenter;
        spGs[i]->ownerGroup = this->self;
        this->updateBounds(spGs[i]);
        refreshRotCenter(this);
        // spGs[i]->subscribeTo(this);
      }
      int supGindex = 0;
      for(supGindex = 0; supGindex < childs.size(); supGindex++){
        if(pool[supGindex] == this) break;
      } 

      return supGindex;
    }

    void SuperGroup::unGroup(std::vector<SuperGroup*> spGs){
      for(int i = 0; i < spGs.size(); i++){

        // std::vector<SpriteGroup*>::iterator it;
        int spGindex = 0;
        for(spGindex = 0; spGindex < childs.size(); spGindex++){
          if(&(childs[spGindex]) == &(spGs[i])) break;
        } 
        if(spGindex != childs.size()){
          delete childs[spGindex];
          childs.erase(childs.begin()+spGindex);
        }  

        spGs[i]->angleFrSuper = false  ;
        spGs[i]->ownerGroup   = nullptr;

        refreshBounds();

      }
      if(this->childs.size() == 0) delete this;
    }
    void SpriteGroup::refreshBounds(){
      Vec2 oldPos = bounds.pos;

      bounds = {0,0,0,0};
      for(int i = 0; i < sprites.size(); i++){
        SpriteGroup::updateBounds(sprites[i]);
      }

      if(bounds.pos != oldPos && !ignoreCamera){
        worldPos.x += bounds.pos.x - oldPos.x;
        worldPos.y += bounds.pos.y - oldPos.y;
      }
      // careful not modifying this before previous if
      bounds.pos.x += bounds.pos.x - oldPos.x;
      bounds.pos.y += bounds.pos.y - oldPos.y;
    }

    void SuperGroup::refreshBounds(){
      Vec2 oldPos = bounds.pos;

      bounds = {0,0,0,0};
      for(int i = 0; i < childs.size(); i++){
        SuperGroup::updateBounds(childs[i]);
      }

      bounds.pos.x += bounds.pos.x - oldPos.x;
      bounds.pos.y += bounds.pos.y - oldPos.y;
    }

    void SpriteGroup::placeSprites(std::vector<MTR::RND::Image*> spr){
      for(int i = 0; i < spr.size(); i++){
        sprites.push_back(spr[i]);
        spriteDfr.emplace((void*)spr[i]->self, *spr[i]);
      }
    }

    void SpriteGroup::placeSprite(MTR::RND::Image* spr){
      sprites.push_back(spr);
      spriteDfr.emplace((void*)spr->self, *spr);
    }
    
    void SpriteGroup::removeSprite(MTR::RND::Image* spr){
      // delete spr;
      sprites.erase(std::remove(sprites.begin(), sprites.end(), spr), sprites.end());
      std::map<void *, MTR::RND::Image>::iterator it = spriteDfr.find((void*)&spr);
      if(it == spriteDfr.end()){

      } else {
        it->second.pendingErase = true;
      }
      // (spriteDfr[(void*)&spr]).pendingErase = true;
    }

    void SpriteGroup::removeSprites(std::vector<MTR::RND::Image*> spr){
      for(int i = 0; i < spr.size(); i++){
        removeSprite(spr[i]);
      }
    }

    // void SpriteGroup::placeSprite(void* ptr, MTR::RND::Image* spr){
    //   sprites.emplace(ptr, spr);
    // }    

    void SuperGroup::updateBounds(Renderable* rend){
      // Vec2 minOffset = {INT_MAX,INT_MAX};
      // Box groupBox = {0,0};
      bounds.box.width  = 
      std::max(rend->bounds.box.width  + rend->bounds.pos.x, bounds.box.width  + bounds.pos.x);
      bounds.box.height = 
      std::max(rend->bounds.box.height + rend->bounds.pos.y, bounds.box.height + bounds.pos.y);

      // Vec2 oldPos = bounds.pos;
      // TODO :: WorldPos?
      bounds.pos.x = 
      std::min(bounds.pos.x, bounds.pos.x + (rend->bounds.pos.x - bounds.pos.x));
      bounds.pos.y = 
      std::min(bounds.pos.y, bounds.pos.y + (rend->bounds.pos.y - bounds.pos.y));
    }


    // void Renderable::deepCopy(Renderable* source, Renderable* target){
    //   target->bounds       = source->bounds       ;
    //   target->windows      = source->windows      ;
    //   target->hidden       = source->hidden       ;
    //   target->angle        = source->angle        ;
    //   target->rotCenter    = source->rotCenter    ;
    //   target->zoomCenter   = source->zoomCenter   ;
    //   target->zoom         = source->zoom         ;
    //   target->flip         = source->flip         ;
    //   target->angleFrSuper = source->angleFrSuper ;
    //   target->ownerGroup   = source->ownerGroup   ;
    // }

    // void Image::deepCopy(Image* source, Image* target){
    //     Renderable::deepCopy(source, target);

    //     target->area     = source->area;
    //     target->layer    = source->layer;
    //     target->tgtRect  = source->tgtRect;
    //     target->srcRect  = source->srcRect;
    //     target->fileName = source->fileName;
    //     target->textures = source->textures;
    // }


    // void SpriteGroup::deepCopy(SpriteGroup* source, SpriteGroup* target){
    //   SuperGroup::deepCopy(source, target);

    //   target->layer        = source->layer;
    //   target->ignoreCamera = source->ignoreCamera;
    //   target->worldPos     = source->worldPos;
    //   target->spriteDfr    = source->spriteDfr;
    // }


    // void SuperGroup::deepCopy(SuperGroup* source, SuperGroup* target){
    //     Renderable::deepCopy(source, target);
    // }

    // void Text::deepCopy(Text* source, Text* target){
    //     Image::deepCopy(source, target);

    //     target->font      = source->font;
    //     target->size      = source->size;
    //     target->textColor = source->textColor;
    // }


  }

    // DeferBuffer& DeferBuffer::operator<<(DeferBuffer&& old_obj){
    //   Buffer::addUpdates(&old_obj, this);

    //   // dfrLayer       = old_obj.dfrLayer      ;
    //   // dfrSpriteGroup = old_obj.dfrSpriteGroup;
    //   // dfrSuperGroup  = old_obj.dfrSuperGroup ;
    //   return *this;
    // };

}

