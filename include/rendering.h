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


  class Rendering {
    public:
    struct SDLSurface;
    struct SDLFont;
    class Renderable;
    class Window;

    protected:
    inline static std::string currentWindow;
    inline static std::map<std::string, SDLSurface> surfaces;
    inline static std::map<std::string, SDLFont> fonts;

    std::thread renderThread;

    // inline static std::string workingWindow;
    inline static std::map<std::string, std::shared_ptr<Window>> windows;
    // static void setWorkingWindow(std::string window){workingWindow = window;};

    struct GroupedMovements {
      Vec2 newPos;
      std::vector<std::shared_ptr<Renderable>> renderables = std::vector<std::shared_ptr<Renderable>>();
      bool worldPos = false;
    };


    private:

    void mane(){
      // std::map<std::string, std::shared_ptr<SDLA::Rendering::Window>>::iterator it;
      while (true) {
        if(windows.size() != 0) windows[currentWindow]->display();
        //   for(it = windows.begin(); it != windows.end(); it++){
        //     if (it->second->pendingErase) {
        //       it = windows.erase(it);
        //     } else {

        //       it->second->display();

        //       it++;
        //     }
        //   }
        // }
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

    struct Info{
      std::atomic<bool> hidden = false;
      std::atomic<int> angle = 0;
      Vec2* rotCenter = nullptr;
      Vec2 offset = {0,0};
      std::atomic<bool> pendingErase = false;
      std::atomic<bool> ignoreCamera = true;
      // int layer;
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
    };

    struct SpriteInfo : Info{
      Bounds area;
      SDL_RendererFlip flip = SDL_FLIP_NONE;
      std::string fileName = "assets/gen_specialchest.png";
      std::shared_ptr<SpriteGroup> ownerGroup = nullptr;
    };

    typedef struct {
      SpriteInfo* info;
      std::string textureText;
      std::string fontName;
      int size;
      SDL_Color textColor;
    } TextInfo;

    Rendering(){
      renderThread = std::thread(mane, this);
    }

    ~Rendering(){
      renderThread.detach();
      windows = {};};

    std::string newWindow(
        int layerCount,
        Box windowSize,
        std::string name = "Abstraction",
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE
    );
    static void setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG);
    static void setGroupRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG, Vec2 center);
    // static const std::string getCurrentWindow(){return currentWindow;};
    static const std::string getCurrentWindowName(){return currentWindow;};
    static const std::shared_ptr<Window> getCurrentWindow(){return windows[currentWindow];};
    static const std::shared_ptr<Window> getWindow(std::string drawWindow){return windows[drawWindow];};
    // static const std::mutex getDrawWindowMutex(std::string drawWindow){return windows[drawWindow]->;};
    // static std::string getWorkingWindow(){return workingWindow;}

    class Window {
      protected:
      std::vector<GroupedMovements> groupedMovements = std::vector<GroupedMovements>();
      // struct Layer;


      private:
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
      std::mutex mutex;


      Vec2 imageOffset;
      Vec2 camPos = {0,0};

      // ~Window();

      Window(
        int layerCount,
        Box windowSize,
        std::string name,
        Vec2 position,
        SDL_WindowFlags mode
        );


      //Layer methods
      void hideLayer(int layer){writeBuffer[layer]->hidden = true;};
      void showLayer(int layer){writeBuffer[layer]->hidden = false;};
      void offsetLayer(int layer, Vec2 offset){writeBuffer[layer]->offset = offset;};

      const std::vector<std::shared_ptr<Layer>> getBuffer(){return writeBuffer;};
      int clampLayerIndex(int layer){
        return std::max(std::min(layer,getLayerCount()-1),0);
      }

      int getLayerCount(){return writeBuffer.size();};
      std::shared_ptr<Layer> getLayer(int layer){return writeBuffer[clampLayerIndex(layer)];};
      // std::vector<std::shared_ptr<SDLA::Rendering::Window::Layer>> getWriteBuffer(){return writeBuffer;};

      // inline static std::shared_ptr<Window> currentWindow;
      // static ID getCurrentWindowID(){return focusedWin;};
      // ID getID(){return window->winID;};

      // SDL_Renderer* const getContext(){return context;};
      // SDL_Texture* createTexture(SDL_Surface* surface);
      // int const getThread(){return threadID;};
      void display();

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
      Renderable(SpriteInfo* info, int layer, std::string window, std::shared_ptr<SpriteGroup> ownerGroup){
        // this->ignoreCamera = ignoreCamera;
        // std::shared_ptr<SpriteInfo> sprI = std::make
        this->info = std::shared_ptr<SpriteInfo>(info);
        this->layer = windows[window]->clampLayerIndex(layer);
        // this->info->layer = this->layer;
        this->myWindow = window;
        this->ownerGroup = ownerGroup;
        // this->fileName = info->fileName;
      };
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
      Text(TextInfo* txtInfo, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

      static std::shared_ptr<Text> loadText(std::string window, int layer, TextInfo* txtInfo, bool ignoreCamera);
      static SDLFont* loadFont(std::string fileName, int size);
      static SDLSurface* loadSurface(std::string textureText, TextInfo* txtInfo);

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
      Sprite(Rendering::SpriteInfo* info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);
      Sprite(std::shared_ptr<Rendering::SpriteInfo> info, int layer, bool ignoreCamera, std::string window, std::shared_ptr<SpriteGroup> ownerGroup);

      static std::shared_ptr<Sprite> addImage(std::string window, int layer, SpriteInfo* info, bool ignoreCamera);
      // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
      static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, SpriteGroup* groupInfo, std::vector<SpriteInfo*> group);
      static std::vector<std::shared_ptr<Sprite>> addImageGroup(std::string window, int layer, SpriteGroup* groupInfo, std::vector<std::shared_ptr<SpriteInfo>> group);
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