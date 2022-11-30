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
#include "identifier.h"



namespace SDLA {

  class Rendering;
  inline static SDLA::Rendering* render;

  class Rendering {
    // private:
    // protected:
    // public:
    public:
    class Renderable;
    class Window;

    struct SDLSurface;
    struct SDLFont;

    struct Info{
      Vec2 offset;
      bool ignoreCamera = true;
      bool hidden = false;
      int angle = 0;
      Vec2* rotCenter = nullptr;

      std::atomic<bool> pendingErase = false;

      Info(
        Vec2 offset,
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
      }
      Info(Info* info){
        Info(
          info->offset, 
          info->ignoreCamera, 
          info->hidden, 
          info->angle, 
          info->rotCenter);
      }
      Info(std::shared_ptr<Info> info){
        Info(
          info->offset, 
          info->ignoreCamera, 
          info->hidden, 
          info->angle, 
          info->rotCenter);
      }
      void fillInfo (Info* fillMe,
        Info* info
      ){
        fillMe = new Info(info);
      }
      void fillInfo (Info* fillMe,
        std::shared_ptr<Info> info
      ){
        fillMe = new Info(info);
      }
    };

    struct SuperGroup : Info{
      std::shared_ptr<SuperGroup> parentGroup = nullptr;
      // std::shared_ptr<SuperGroup> childGroup = nullptr;

      Vec2 worldPos = {0,0};
      std::atomic<bool> childFramesDrawn = false;
    };


    struct SpriteGroup : Info {
      // Vec2 offset ??
      std::shared_ptr<SuperGroup> superGroup = nullptr;
      // std::shared_ptr<SpriteInfo> info;
      std::vector<std::shared_ptr<Renderable>> sprites;

      SpriteGroup();
      std::shared_ptr<SpriteGroup> createSpriteGroup(
        Info* info
        // Vec2 offset,
        // bool ignoreCamera = true,
        // bool hidden = false,
        // int angle = 0,
        // Vec2* rotCenter = nullptr
      ){
        fillInfo(this, info);
        return std::shared_ptr<SpriteGroup>(this);
      }

    };

    struct TextInfo : Info{
      std::string text;
      std::string fontName;
      int size;
      SDL_Color textColor;

      TextInfo();

      std::shared_ptr<TextInfo> create(
        Info* info,
        std::string text,
        std::string fontName,
        int size,
        SDL_Color textColor
      ){
        fillInfo(this, info);
        this->text = text;
        this->fontName = fontName;
        this->size = size;
        this->textColor = textColor;
        return std::shared_ptr<TextInfo>(this);
      }
    };

    struct SpriteInfo : Info{
      Bounds area;
      SDL_RendererFlip flip;
      std::string fileName;
      std::shared_ptr<SpriteGroup> ownerGroup = nullptr;
      
      SpriteInfo();

      std::shared_ptr<SpriteInfo> share(){return std::make_shared<SpriteInfo>(this)};     

      std::shared_ptr<SpriteInfo> createSpriteInfo(
        Info* info,
        Bounds area,
        std::string fileName = "assets/gen_specialchest.png",
        SDL_RendererFlip flip = SDL_FLIP_NONE
      ){
        fillInfo(this, info);
        this->area = area;
        this->fileName = fileName;
        this->flip = flip;
        return std::shared_ptr<SpriteInfo>(this);
      }
      std::shared_ptr<SpriteInfo> createSpriteInfo(
        std::shared_ptr<TextInfo> info,
        SDL_RendererFlip flip = SDL_FLIP_NONE
      ){
        fillInfo(this, info);
        this->flip = flip;
        return std::shared_ptr<SpriteInfo>(this);
      }
    };

    protected:

    inline static std::string currentWindow;
    inline static std::map<std::string, SDLSurface> surfaces;
    inline static std::map<std::string, SDLFont> fonts;

    inline static std::map<std::string, std::shared_ptr<Window>> windows;



    // struct GroupedMovements {
    //   Vec2 newPos;
    //   std::vector<std::shared_ptr<Renderable>> renderables = std::vector<std::shared_ptr<Renderable>>();
    //   bool worldPos = false;
    // };


    private:

    void mane(){
      std::map<std::string, std::shared_ptr<SDLA::Rendering::Window>>::iterator it;
      while (true) {
        if(windows.size() != 0) {/* windows[currentWindow]->display(); */
          for(it = windows.begin(); it != windows.end(); it++){

            if (it->second == nullptr ||
                it->second->pendingErase) {

              it = windows.erase(it);
              continue;

            } else if (!it->second->getHasOwnThread()) {

              it->second->display();

            }
            it++;
          }
        }
      }
    }
    

    public:

    struct SDLSurface{
      std::string fileName;

      SDL_Surface* sur;
      int useCount;
    };

    struct SDLFont{
      std::string fileName;
      TTF_Font* font;
      int size;
    };

    class Sprite;
    class Renderable;


    // Rendering(){    }

    // ~Rendering(){

      // windows = {};};

    static std::string newWindow(
        int layerCount,
        Box windowSize,
        std::string name = "Abstraction",
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE,
        bool hasOwnThread = false
    );
    static void setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG);
    static void setGroupRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG, Vec2 center);
    // static const std::string getCurrentWindow(){return currentWindow;};
    static const std::string getCurrentWindowName(){return currentWindow;};
    static const std::shared_ptr<Window> getCurrentWindow(){return windows[currentWindow];};
    static const std::shared_ptr<Window> getWindow(std::string drawWindow){return windows[drawWindow];};
    static const int getWindowCount(){return windows.size();};
    static void pollWindowEvents(SDL_Event event){
      std::map<std::string, std::shared_ptr<SDLA::Rendering::Window>>::iterator it;
      if(windows.size() != 0) {/* windows[currentWindow]->display(); */
        for(it = windows.begin(); it != windows.end(); it++){
          it->second->pollEvents(event);
        }
      }
    }
    // static const std::mutex getDrawWindowMutex(std::string drawWindow){return windows[drawWindow]->;};
    // static std::string getWorkingWindow(){return workingWindow;}

    class Window {
      protected:
      // std::vector<GroupedMovements> groupedMovements = std::vector<GroupedMovements>();
      // struct Layer;


      private:
      void mane(){
        while(!pendingErase) windows[name]->display();

        windows[name] = nullptr;

      }
      std::thread renderThread;
      bool hasOwnThread = false;

      int ms = 0;
      int fps = 0;
      bool fpsShown;

      SDL_WindowFlags windowFlags;
      struct Layer{
        std::vector<std::shared_ptr<SpriteGroup>> groups = std::vector<std::shared_ptr<SpriteGroup>>();

        ID id;
        Vec2 offset = {0,0};
        std::atomic<bool> hidden = false;
      };

      // struct DoubleBuffer {
        std::vector<std::shared_ptr<Layer>> readBuffer = std::vector<std::shared_ptr<Layer>>();
        std::vector<std::shared_ptr<Layer>> writeBuffer = std::vector<std::shared_ptr<Layer>>();

      // };

      Box size;

      
      // struct Layers{
      // DoubleBuffer doubleBuffer;

      // };

      // inline static Layers layers;

      // int threadID;

      std::atomic<bool> mouseFocus;
      std::atomic<bool> keyboardFocus;
      std::atomic<bool> fullScreen;
      std::atomic<bool> minimized;
      std::atomic<bool> shown;
      // ID winID;
      SDL_Window* window;
      SDL_Renderer* context;
      std::string name;

      // inline static ID focusedWin;

      // std::mutex mtx;

      void updateGroupBuffer(){
        readBuffer = writeBuffer;
      }

      public:
      // std::atomic<bool> writeBufferBusy = false;    
      const int getBufferSize(){return writeBuffer.size();};

      std::mutex mutex;

      Vec2 imageOffset;
      Vec2 camPos = {0,0};

      Vec2 position;
      // Uint32 windowID;

      // ~Window();

      Window(
        int layerCount,
        Box windowSize,
        std::string name,
        Vec2 position,
        SDL_WindowFlags mode,
        bool hasOwnThread = false
        );


      //Layer methods
      void hideLayer(int layer){writeBuffer[layer]->hidden = true;};
      void showLayer(int layer){writeBuffer[layer]->hidden = false;};
      void offsetLayer(int layer, Vec2 offset){writeBuffer[layer]->offset = offset;};

      const std::vector<std::shared_ptr<Layer>> getBuffer(){return writeBuffer;};
      const bool getHasOwnThread() {return hasOwnThread;};
      SDL_Window* getSDL_Window(){return window;};

      int clampLayerIndex(int layer){
        return std::max(std::min(layer,getLayerCount()-1),0);
      }

      int getLayerCount(){return writeBuffer.size();};
      std::shared_ptr<Layer> getLayer(int layer){return writeBuffer[clampLayerIndex(layer)];};
      // std::vector<std::shared_ptr<SDLA::Rendering::Window::Layer>> getWriteBuffer(){return writeBuffer;};

      // inline static std::shared_ptr<Window> currentWindow;
      // static ID getCurrentWindowID(){return focusedWin;};
      // ID getID(){return window->winID;};

      SDL_Renderer* const getContext(){return context;};
      // SDL_Texture* createTexture(SDL_Surface* surface);
      // int const getThread(){return threadID;};
      void display();
      void pollEvents(SDL_Event event);

      std::atomic<bool> pendingErase = false; 
      void close(){pendingErase = true;};

    };

    // enum RenderTypes{
    //   TEXT,
    //   IMAGE
    // };


    class Renderable {
      public:

      std::atomic<bool> hidden = false;
      std::atomic<bool> pendingErase = false;
      std::atomic<bool> getAngleFromGroup = true;
      bool frameDone = true;


      // void remove(){pendingErase = true;};
      // void hide(){hidden = true;};
      // void show(){hidden = false;};

      // void setAngle(int degrees);
      // void addAngle(int degrees);

      // void setRotationCenter(Vec2 center);

      // std::shared_ptr<SpriteInfo> const getInfo(){return info;};
      SDL_Texture* const getTexture(){return texture;};
      void setCrop(Bounds crop, bool rendererCall = false);
      bool getIgnoreCamera(){return ownerGroup->ignoreCamera;}

      Bounds getBounds();


      // For display loop exclusively
      void const setTexture(SDL_Texture* tex){texture = tex;};

      std::shared_ptr<SpriteInfo> info;
      std::string myWindow;
      std::shared_ptr<Rendering::SpriteGroup> ownerGroup;

      std::atomic<bool> texQueued = true;
      // RenderTypes type;

      SDL_Rect* const getSRCRect(){return &srcRect;};
      SDL_Rect* const getSDLRect(){return &sdlRect;};
      std::string const getFileName(){return info->fileName;};

      protected:
      Renderable(std::shared_ptr<SpriteInfo> info, int layer, std::string window, std::shared_ptr<SpriteGroup> ownerGroup){
        // this->ignoreCamera = ignoreCamera;
        // std::shared_ptr<SpriteInfo> sprI = std::make
        this->info = info;
        this->layer = windows[window]->clampLayerIndex(layer);
        // this->info->layer = this->layer;
        this->myWindow = window;
        this->ownerGroup = ownerGroup;
        // this->fileName = info->fileName;
      };

      SDL_Rect sdlRect;
      SDL_Rect srcRect;
      // std::string fileName;
      // std::atomic<bool> ignoreCamera = true;
      int layer;

      private:
      SDL_Texture* texture;

    };

    class Text : public Renderable{
      public:
      Text(std::shared_ptr<TextInfo> txtInfo, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

      static std::shared_ptr<Text> loadText(std::string window, int layer, std::shared_ptr<SDLA::Rendering::TextInfo> txtInfo, bool ignoreCamera);
      static SDLFont* loadFont(std::string fileName, int size);
      static SDLSurface* loadSurface(std::shared_ptr<TextInfo> txtInfo);

      void updateText(std::string text);
      void changeFont(std::string fontName);
      void resize(int size);
      // void setColor( Uint8 red, Uint8 green, Uint8 blue ){
      //   color = {red,green,blue};
      // };

      //Set blending??
      // void setBlendMode( SDL_BlendMode blending );
      //Set alpha??
      // void setAlpha( Uint8 alpha );

      SDL_Color color;


      // TTF_Font* gFont;

      // SDL_Surface* sur;

      // RenderTypes type = TEXT;

      private:
      ID txtID;

      std::shared_ptr<TextInfo> textInfo;

    };

    class Sprite : public Renderable{
      public:
      // Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);
      Sprite(std::shared_ptr<Rendering::SpriteInfo> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

      static std::shared_ptr<Sprite> addImage(std::string window, int layer, std::shared_ptr<SpriteInfo> info, bool ignoreCamera);
      // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
      // static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, std::shared_ptr<Info> groupInfo, std::vector<SpriteInfo*> group);
      static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, Info* groupInfo, std::vector<std::shared_ptr<SpriteInfo>> group);
      static SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);

      // JEN AI TU BESOIN
      // void setPosition(WorldPos position);
      // void moveFromPosition(Vec2 move);
      // Internal, do not use
      // void updateSDLRect(); // Is this even required?

      void setFlip();

      void changeSurface(std::string fileName);


      // ~Sprite();


      // RenderTypes type = IMAGE;
      
      // std::shared_ptr<SpriteGroup> ownerGroup = nullptr;


      // private:

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