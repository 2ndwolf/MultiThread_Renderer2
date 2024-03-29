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

#include "primitives.h"
#include "identifier.h"


namespace SDLA {


  class Rendering {
    protected:
    struct SDLSurface{
      SDL_Surface* sur;
      int useCount;
    };
    static std::map<std::string, SDLSurface> surfaces;
    std::thread renderThread;
    // static std::thread* getThread(int threadID);

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
    static std::mutex mutex;

    class Sprite;
    class Window;
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

      std::string fileName = "assets/gen_specialchest.png";

      bool ignoreCamera = true;

      // SDLA::ID spriteID;
      // SDLSurface* surface = nullptr;
      // bool changedSurface = false;
      // bool changedArea = false;
    };
    
    typedef struct {
      SpriteInfo* info = new SpriteInfo();
      bool updated = false;
      std::atomic<bool> pendingErase{false};
      std::map<SDLA::ID, std::shared_ptr<Sprite>> sprites;

    } SpriteGroup;

    Rendering(){
      windows = Windows();
      
      // newWindow(1, (SDLA::Box) {500,500});
      renderThread = std::thread(mane, this);
    }

    // TODO fix this
    // ~Rendering(){renderThread.detach();};

    std::shared_ptr<Window> newWindow(
      int layercount,
      Box windowSize,
      std::string name = "Abstraction",
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE
      );

    static SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);

    class Window {
      public:
      SDLA::idInf newGroup(int layer);
      SDLA::ID updImage(SDLA::idInf idInf, Rendering::SpriteInfo* info, SDLA::ID sprID = SDLA::Identifier::noID());
      void updGroup(SDLA::idInf idInf, Rendering::SpriteInfo* info);

      private:
      struct Layer {
        std::map<SDLA::ID, std::shared_ptr<SpriteGroup>> groups;
        std::map<SDLA::ID, std::shared_ptr<SpriteGroup>> groupsMirror;
        Vec2 offset;
        std::atomic<bool> hidden{false};
        SDLA::ID groupCount = 0;
        std::map<SDLA::ID, std::shared_ptr<SpriteGroup>> groupBuffer;
        std::map<SDLA::ID, std::shared_ptr<SpriteGroup>> groupBufferOld;
        std::atomic<bool> gBufferBusy;
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
      SDL_Window* window;
      SDL_Renderer* context;
      SDL_Renderer* context2;
      SDLA::ID windowID;

      // std::mutex mtx;

      void updateGroupBuffer(int layer);

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

      SDL_Renderer* const getContext(){return context;};
      SDL_Texture* createTexture(SDL_Surface* surface);
      // int const getThread(){return threadID;};
      void display();

      void close(){pendingErase = true;};
      std::atomic<bool> pendingErase{false}; 

    };



    class Sprite{
      public:
      static void setWorkingWindow(std::shared_ptr<Window> window){win = window;};

      // static std::shared_ptr<Rendering::Sprite> addImage(int layer, SpriteInfo* info, int groupID = -1); // Add to group 0
      // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
      // static std::shared_ptr<Rendering::SpriteGroup> addImageGroup(int layer, std::vector<SpriteInfo*> group, int groupID = -1);

      void remove(){pendingErase = true;};

      void hide(){hidden = true;};
      void show(){hidden = false;};

      void setAngle(int degrees);
      void addAngle(int degrees);
      void setRotationCenter(Vec2 center);
      static void setGroupAsRotationCenter(std::shared_ptr<Rendering::SpriteGroup> sG);

      void setCrop(Bounds crop, bool rendererCall = false);

      void setPosition(WorldPos position);
      void moveFromPosition(Vec2 move);
      WorldPos getPosition(){return info->pos.worldPos;};
      // Internal, do not use
      void updateSDLRect(); // Is this even required?

      void setFlip();

      void changeSurface(std::string fileName);

      SDL_Texture* const getTexture(){return texture;};
      SDL_Rect* const getSRCRect(){return &srcRect;};
      SDL_Rect* const getSDLRect(){return &sdlRect;};
      SpriteInfo* const getInfo(){return info;};
      // void render();
      std::atomic<bool> pendingErase{false};
      // ~Sprite();
      Sprite(SpriteInfo* info);

      std::atomic<bool> texQueued{true};
      std::string fileName;
      SDL_Texture* texture;
      SDLA::ID ownerGroupID;
      // std::atomic<bool> texReady = false;


      private:
      SpriteInfo* info;
      static std::mutex mtx;
      static std::shared_ptr<Window> win;
      std::shared_ptr<Window> myWin;
      std::atomic<bool> hidden{false};
      SDL_Rect sdlRect;
      SDL_Rect srcRect;
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