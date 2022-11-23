#ifndef _SDLA_rendering_h
#define _SDLA_rendering_h

#include <vector>
#include <thread>
#include <string>
#include <map>
#include <atomic>
#include <mutex>
#include <memory>

// Implementation
#include <algorithm>

#include <SDL.h> 
#include <SDL_image.h> 
#include <SDL_ttf.h>

#include "primitives.h"



namespace SDLA {


  class Rendering {
    protected:
    struct SDLSurface{
      std::string fileName;

      SDL_Surface* sur;
      int useCount;
    };
    inline static std::map<std::string, SDLSurface> surfaces;
    struct SDLFont{
      std::string fileName;
      TTF_Font* font;
      int size;
    };
    inline static std::map<std::string, SDLFont> fonts;
    std::thread renderThread;



    private:
    // static int threadCount;
    // int threadID;

    void mane(){
      while (true) {
        if(windows.members.size() != 0){
          windows.members.erase(
            std::remove_if(std::begin(windows.members), std::end(windows.members),
              [&] (std::shared_ptr<SDLA::Rendering::Window> win) -> bool {
                if (win->pendingErase) {
                  return true;
                }
                win->display();
                return false;
              }
            ),
            std::end(windows.members)
          );
        }
      }
    }
    
    public:
    inline static std::mutex mutex;

    class Sprite;
    class Window;
    class Renderable;
    struct Windows{
      std::vector<std::shared_ptr<Window>> members;
      // int threadID;
    };

    SDLA::Rendering::Windows windows;

    struct Position{
      WorldPos worldPos;
      Vec2 offset;
    };

    struct SpriteInfo{
      bool hidden = false;
      int angle = 0;
      Vec2* rotCenter = nullptr;
      Bounds area;


      Position pos;
      SDL_RendererFlip flip = SDL_FLIP_NONE;
      // SDLSurface* surface = nullptr;

      std::string fileName = "assets/gen_specialchest.png";

      bool ignoreCamera = true;

      // bool changedSurface = false;
      // bool changedArea = false;
    };
    
    typedef struct {
      SpriteInfo* info = new SpriteInfo();
      std::atomic<bool> pendingErase = false;
      std::vector<std::shared_ptr<Renderable>> sprites;
    } SpriteGroup;

    Rendering(){
      windows = Windows();
      renderThread = std::thread(mane, this);
    }

    ~Rendering(){renderThread.detach();};

    std::shared_ptr<Window> newWindow(
      int layercount,
      Box windowSize,
      std::string name = "Abstraction",
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE
      );

    static SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);
    static SDLFont* loadFont(std::string fileName, int size);
    static SDLSurface* loadTextSurface(std::string textureText, TTF_Font* font, SDL_Color textColor);


    class Window {
      private:
      struct Layer {
        std::vector<std::shared_ptr<SpriteGroup>> groups = std::vector<std::shared_ptr<SpriteGroup>>();
        std::vector<std::shared_ptr<SpriteGroup>> groupBuffer = std::vector<std::shared_ptr<SpriteGroup>>();

        Vec2 offset;
        std::atomic<bool> hidden = false;
        int groupCount = 0;
      };

      Box size;
      std::vector<std::shared_ptr<Layer>> layers;
      int layerCount;

      // int threadID;

      std::atomic<bool> mouseFocus;
      std::atomic<bool> keyboardFocus;
      std::atomic<bool> fullScreen;
      std::atomic<bool> minimized;
      std::atomic<bool> shown;
      ID winID;
      SDL_Window* window;
      SDL_Renderer* context;
      SDL_Renderer* context2;
      inline static ID focusedWin;

      // std::mutex mtx;

      void updateGroupBuffer(int layer){
        layers[layer]->groups = layers[layer]->groupBuffer;
      }

      public:

      // ~Window();
      Window(
        int layerCount,
        Box windowSize,
        std::string name = "Abstraction",
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE
        );


      //Layer methods
      // void removeLayer(int layer){layers[layer].pendingErase = true;};
      void hideLayer(int layer){layers[layer]->hidden = true;};
      void showLayer(int layer){layers[layer]->hidden = false;};
      void offsetLayer(int layer, Vec2 offset){layers[layer]->offset = offset;};

      int getLayerCount(){return layerCount-1;};
      std::shared_ptr<Layer> getLayer(int layer){return layers[layer];};

      static ID getCurrentWindowID(){return focusedWin;};
      static ID getWindowID(std::shared_ptr<Window> window){return window->winID;};

      SDL_Renderer* const getContext(){return context;};
      SDL_Texture* createTexture(SDL_Surface* surface);
      // int const getThread(){return threadID;};
      void display();

      void close(){pendingErase = true;};
      std::atomic<bool> pendingErase = false; 

    };

    // enum RenderTypes{
    //   TEXT,
    //   IMAGE
    // };

    class Renderable {
      public:
      std::atomic<bool> hidden = false;
      std::atomic<bool> pendingErase = false;

      static void setWorkingWindow(std::shared_ptr<Window> window){win = window;};

      void remove(){pendingErase = true;};
      void hide(){hidden = true;};
      void show(){hidden = false;};

      void setAngle(int degrees);
      void addAngle(int degrees);

      void setRotationCenter(Vec2 center);
      static void setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG);

      SpriteInfo* const getInfo(){return info;};
      SDL_Texture* const getTexture(){return texture;};
      void const setTexture(SDL_Texture* tex){texture = tex;};
      void setCrop(Bounds crop, bool rendererCall = false);

      inline static std::mutex mtx;

      SpriteInfo* info;
      inline static std::shared_ptr<Window> win;
      std::shared_ptr<Window> myWin;

      std::atomic<bool> texQueued = true;
      // RenderTypes type;

      SDL_Rect* const getSRCRect(){return &srcRect;};
      SDL_Rect* const getSDLRect(){return &sdlRect;};

      SDL_Rect sdlRect;
      SDL_Rect srcRect;
      private:
      SDL_Texture* texture;
    };

    class Text : public Renderable{
      public:
      Text(SpriteInfo* info, std::string textureText, std::string fontName, int size, SDL_Color textColor);
      static std::shared_ptr<Rendering::SpriteGroup> loadText(std::shared_ptr<Window> window, int layer, SpriteInfo* info, std::string textureText, std::string fontName, int size, SDL_Color textColor);
      void setColor( Uint8 red, Uint8 green, Uint8 blue );
      //Set blending??
      void setBlendMode( SDL_BlendMode blending );
      //Set alpha modulation??
      void setAlpha( Uint8 alpha );

      SDL_Color color;

      // TTF_Font* gFont;

      // SDL_Surface* sur;

      // RenderTypes type = TEXT;

    };

    class Sprite : public Renderable{
      public:
      static std::shared_ptr<Rendering::SpriteGroup> addImage(std::shared_ptr<SDLA::Rendering::Window> window, int layer, SpriteInfo* info);
      // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
      static std::shared_ptr<Rendering::SpriteGroup> addImageGroup(std::shared_ptr<SDLA::Rendering::Window> window, int layer, SpriteInfo* groupInfo, std::vector<SpriteInfo*> group);

      // JEN AI TU BESOIN
      void setPosition(WorldPos position);
      void moveFromPosition(Vec2 move);
      WorldPos getPosition(){return info->pos.worldPos;};
      // Internal, do not use
      void updateSDLRect(); // Is this even required?

      void setFlip();

      void changeSurface(std::string fileName);


      // ~Sprite();
      Sprite(SpriteInfo* info);

      std::string fileName;

      // RenderTypes type = IMAGE;
      
      // std::shared_ptr<SpriteGroup> ownerGroup = nullptr;


      private:


    };

    /**
     * Class Window with cam : public Rendering::Window{
     *   WindowWithCam() : initlayers{
     *    Rendering::Window::init();
     *    attachToWindowList();
     *    }
     * };
     * */


  };

}
#endif