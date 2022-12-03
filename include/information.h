#ifndef _FK_information_h_
#define _FK_information_h_

#include <atomic>
#include <memory>
#include <vector>
#include <string>

#include <SDL.h>
#include <SDL_image.h> 
#include <SDL_ttf.h>

#include "primitives.h"
#include "fkenum.h"
// #include "renderable.h"


namespace FK {
  namespace ORE {
    class Renderable;
  }
  namespace AT  {
    // class Renderable;
    // class Information;
    // class SuperGroup;
    // class SpriteGroup;
    // class SpriteInformation;
    // class TextInformation;

    class Information{
      public:
      Vec2 offset = {0,0};
      bool ignoreCamera = true;
      bool hidden = false;
      int angle = 0;
      Vec2* rotCenter = nullptr;

      std::atomic<bool> pendingErase = false;

      // Information(){
      //   offset = {0,0};
      // };

      // Information(Information information){
      //   this->offset        = information.offset;
      //   this->ignoreCamera  = information.ignoreCamera;
      //   this->hidden        = information.hidden;
      //   this->angle         = information.angle;
      //   this->rotCenter     = information.rotCenter;
      // };

      Information(
        Vec2 offset = Vec2{0,0},
        bool ignoreCamera = true,
        bool hidden = false,
        int angle = 0,
        Vec2* rotCenter = nullptr
      ){
        this->offset = offset;
        this->ignoreCamera = ignoreCamera;
        this->hidden = hidden;
        this->angle = angle;
        this->rotCenter = rotCenter;
      };

      inline void fillMe(Information* child);
    };
    void Information::fillMe(Information* child){
      child->offset        = this->offset;
      child->ignoreCamera  = this->ignoreCamera;
      child->hidden        = this->hidden;
      child->angle         = this->angle;
      child->rotCenter     = this->rotCenter;
    };

    class SuperGroup : public Information {
      public:
      inline SuperGroup();
      inline SuperGroup(std::shared_ptr<Information> information, bool isRotationCenter = false);
      // SuperGroup(Information information) : Information(information){};

      std::shared_ptr<SuperGroup> parentGroup = nullptr;
      // std::shared_ptr<SuperGroup> childGroup = nullptr;
      bool isRotationCenter = false;
    };
    SuperGroup::SuperGroup(){};
    SuperGroup::SuperGroup(std::shared_ptr<Information> information, bool isRotationCenter){
      information->fillMe(this);
      this->isRotationCenter = isRotationCenter;
    };


    class SpriteGroup : public SuperGroup {
      public:
      inline SpriteGroup();
      inline SpriteGroup(std::shared_ptr<Information> information, bool isRotationCenter = false);
      // SpriteGroup(Information information) : Information(information){};

      std::shared_ptr<SuperGroup> superGroup = nullptr;
      std::vector<std::shared_ptr<ORE::Renderable>> sprites;

      Vec2 worldPos = {0,0};

    };
    SpriteGroup::SpriteGroup(){};
    SpriteGroup::SpriteGroup(std::shared_ptr<Information> information, bool isRotationCenter) : SuperGroup(std::make_shared<Information>(), isRotationCenter){
      information->fillMe(this);
      // this->isRotationCenter = isRotationCenter;
    };


    class SpriteInformation : public Information {
      public:
      Bounds area = {0,0};
      std::string fileName = "assets/gen_specialchest.png";
      ENUM::Flip flip = ENUM::Flip::NONE;
      std::shared_ptr<SpriteGroup> ownerGroup = std::make_shared<SpriteGroup>();

      inline SpriteInformation();

      inline SpriteInformation(std::shared_ptr<Information> information,
        Bounds pArea = {0,0},
        std::string pFileName = "assets/gen_specialchest.png",
        ENUM::Flip pFlip = ENUM::Flip::NONE
      ) : area(pArea), fileName(pFileName), flip(pFlip){
        information->fillMe(this);
      };

      std::shared_ptr<SpriteInformation> share(){
        return std::shared_ptr<SpriteInformation>(this);
      };
    };
    SpriteInformation::SpriteInformation(){};
    // SpriteInformation::SpriteInformation(std::shared_ptr<Information> information,
    //     Bounds area,
    //     std::string fileName,
    //     ENUM::Flip flip
    //   ){
    //     information->fillMe(this);
    //   };


    class TextInformation : public Information {
      public:
        std::string text;
        std::string fontName;
        int size;
        SDL_Color textColor;

      inline TextInformation(std::shared_ptr<Information> information,
        std::string text,
        std::string fontName,
        int size,
        SDL_Color textColor
      );
      inline TextInformation(Information information,
        std::string text,
        std::string fontName,
        int size,
        SDL_Color textColor
      );
      
    };
    TextInformation::TextInformation(std::shared_ptr<Information> information,
      std::string text,
      std::string fontName,
      int size,
      SDL_Color textColor
    ){
      information->fillMe(this);;;
      this->text =           text;
      this->fontName =   fontName;
      this->size =           size;
      this->textColor = textColor;
    };
    // TextInformation::TextInformation(Information information,
    //   std::string text,
    //   std::string fontName,
    //   int size,
    //   SDL_Color textColor
    // ){
    //   information.fillMe(this);
    //   this->text = text;
    //   this->fontName = fontName;
    //   this->size = size;
    //   this->textColor = textColor;
    // };
  }
}


#endif