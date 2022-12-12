/**
 * 
 * MTR
 * 
 * Multi Thread Renderer (v2)
 * cuz once I rebased my local version with an old one ToT
 * 
 * This is the library header. Only this header should be
 * necessary to interface with an MTR lib.
 * 
 * (C) MTR's license is the same as SDL's cuz it uses it.
 * 
 * License is subject to change once I get rid of the SDL
 * dependency and write my own rendering back-back-back-end
 * 
 * 
 * Use:
 * MTR_Init() to initialize MTR
 * MTR_Close() to close it! (It shall do its cleaning chores someday)
 * 
 * MTR::newWindow()   to create a window
 * 
 * MTR::createImage() to display an image
 * MTR::createText()  to display text
 * 
 * STORE THOSE SOMEWHERE!!!!
 * Because when a change is made you gotta call:
 * update(myImage) or update(myText) or wtv
 * on them
 * 
 * If a change is made after an update, it will be lost for the
 * renderer.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/



#ifndef _MTR_LIBRARY_h_
#define _MTR_LIBRARY_h_

#include <string>
#include <memory>
#include <vector>
#include <cstdint>

int MTR_init();
int MTR_close();


typedef struct Vec2{
  int x = 0, y = 0;

  bool operator==(const Vec2& comp){
    return x == comp.x && y == comp.y;
  }
} Vec2;

typedef struct Box{
  int width = 0, height = 0;

  bool operator==(const Box& comp){
    return width == comp.width && height == comp.height;
  }
  
} Box;

typedef struct Bounds{
  Vec2 pos;
  Box box;

  bool operator==(const Bounds& comp){
    return box == comp.box && pos == comp.pos;
  } 

} Bounds;


namespace MTR{

  struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };

  namespace SUR{
    struct SDLSurface{
      // std::string fileName;

      // SDL_Surface* sur;
      // int useCount = 0;

      // ~SDLSurface(){
      //   SDL_FreeSurface(sur);
      //   delete sur;
      // }
    };

    struct Font{
      // std::string fileName;
      // TTF_Font* font;
      // int size; 

      // ~Font(){
      //   TTF_CloseFont(font);
      //   delete font;
      // }
    };
  }

  class Window{ public:
    static std::string newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      bool hasOwnThread = false,
      Vec2 position = {50, 50}
    );

    const int getLayerCount(std::string windowName);
    static const std::string getCurrentWinName();
    // static MTR::Window* getCurrentWindow();
    static MTR::Window* getWindow(std::string windowName);
    static const int getWindowCount();

    static void updateAll();
    static void updateOne(std::string window);

  };

  namespace RND{
    class Image      ;
    class SuperGroup ;
    class SpriteGroup;
    class Text;
  }

  RND::Image* createImage(std::string fileName, Vec2 offset, std::vector<std::string> windows);
  RND::Text*  createText(SUR::Font font, std::string text,Vec2 offset, std::vector<std::string> windows, Color color);
  RND::Image* createSprite(std::string fileName, Vec2 offset, Bounds crop, int layer, std::vector<std::string> windows);




  namespace ENUM{
    enum Flip{
      NONE      ,
      HORIZONTAL,
      VERTICAL  
    };
  }

  namespace RND{
    class Image      ;
    class SuperGroup ;
    class SpriteGroup;

    struct Layer{
      // inline Layer(){};

      int  layerNumber;
      Vec2 offset     ;
      bool hidden     ;
      std::vector<std::string> windows;

      void update(Layer layer);
    };

    class Renderable{ public:
      std::vector<std::string> windows = {};
      Bounds     bounds       = {INT_MIN, INT_MIN, 0, 0};
      bool         hidden       = false             ;
      int          angle        = 0                 ;
      Vec2         rotCenter    = {INT_MAX, INT_MAX};
      Vec2         zoomCenter   = {INT_MAX, INT_MAX};
      Vec2         zoom         = {100,100}         ;
      ENUM::Flip   flip         = ENUM::Flip::NONE  ;
      bool         angleFrSuper = true              ;
      SuperGroup*  ownerGroup   = nullptr           ;
      bool         pendingErase = false             ;
    };


    class Image : public Renderable{ public:
      void setSurface(std::string fileName);
      static void update(Image* image);

      static void group(SpriteGroup* sG, std::vector<Image*> imgs);
      void group(SpriteGroup* sG);

      void setCrop(Bounds crop);

      Bounds area = {0,0,0,0};
      int layer = 0;
      std::string fileName;
    };

    class SuperGroup : public Renderable { public:
      SuperGroup(std::vector<std::string> windows);
      static void update(SuperGroup* group);
      void  refreshBounds(Renderable* rend);
    };

    class SpriteGroup: public SuperGroup{ public:
      int  layer       ;
      bool ignoreCamera;
      Vec2 worldPos    ;
      SpriteGroup(std::vector<std::string> windows);
      static void update(SpriteGroup* group);

      void refreshBounds(Renderable* rend);

      void placeSprites(std::vector<MTR::RND::Image*> spr);
      void placeSprite(MTR::RND::Image* spr);
      void placeSprite(void* ptr, MTR::RND::Image* spr);


    };
    
    class Text : public Image { public:
      void setSurface    (std::string newText);
      inline void setText(std::string newText){setSurface(newText);};

      static void update(Text* image);
      static SUR::Font loadFont(std::string fileName, int size);

      SUR::Font font;
      int size;
      Color textColor;
    };
  }
}

#endif