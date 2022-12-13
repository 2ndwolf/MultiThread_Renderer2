#include <string>
#include <vector>

#include "INST.h"
#include "image.h"
#include "text.h"
#include "primitives.h"
 

namespace MTR{

  RND::Image* createImage(const std::string& fileName,
  Vec2 offset,
  int layer,
  std::vector<std::string> windows){
    RND::Image* img = new RND::Image(windows);

    // img->windows    = windows;
    img->bounds.pos = offset ;
    img->layer      = layer  ;

    img->setSurface(fileName);

    img->bounds.box.width  = img->area.box.width ;
    img->bounds.box.height = img->area.box.height;

    img->srcRect.x = img->area.pos.x;
    img->srcRect.y = img->area.pos.y;
    img->srcRect.w = img->area.box.width;
    img->srcRect.h = img->area.box.height;

    img->tgtRect.w = img->area.box.width;
    img->tgtRect.h = img->area.box.height;

    // RND::Image::update(img);

    return img;
  }

  RND::Image* createSprite(const std::string& fileName,
  Vec2 offset,
  Bounds crop,
  int layer,
  std::vector<std::string> windows){
    RND::Image* img = new RND::Image(windows);

    // img->windows    = windows;
    img->bounds.pos = offset ;
    img->layer      = layer  ;

    img->setSurface(fileName, true);

    img->setCrop(crop);

    // RND::Image::update(img);

    return img;
  }

  RND::Text* createText(
    SUR::Font font, 
    const std::string& text,
    Vec2 offset, 
    int layer,
    std::vector<std::string> windows, 
    Color color){
    
    RND::Text* txt = new RND::Text(windows);

    txt->font       = font   ;
    txt->bounds.pos = offset ;
    // txt->windows    = windows;
    txt->textColor  = color  ;
    txt->layer      = layer  ;

    txt->setSurface(text);

    txt->srcRect.x = txt->area.pos.x;
    txt->srcRect.y = txt->area.pos.y;
    txt->srcRect.w = txt->area.box.width;
    txt->srcRect.h = txt->area.box.height;

    txt->tgtRect.w = txt->area.box.width;
    txt->tgtRect.h = txt->area.box.height;


    // RND::Text::update(txt);

    return txt;

  }

  // void setText

}


#define NONONO
#ifndef NONONO
    Text::Text(std::shared_ptr<TextInformation> txtInfo, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup)
    : ORE::Renderable(std::make_shared<SpriteInformation>(txtInfo), layer, window, ownerGroup){
      // txtID = FK::ORE::Identifier::newtxtID();

      FKORE::SDLSurface* txtSur = Text::loadSurface(txtInfo);
      // this->information->fileName = txtSur->fileName;
      // this->textInformation = txtInfo;

      srcRect.x = information->area.pos.x;
      srcRect.y = information->area.pos.y;


      if(txtSur->sur != NULL){
        srcRect.w = txtSur->sur->w;
        srcRect.h = txtSur->sur->h;
        sdlRect.w = txtSur->sur->w;
        sdlRect.h = txtSur->sur->h;
      }

      sdlRect.x = 0;
      sdlRect.y = 0;
    }
#endif