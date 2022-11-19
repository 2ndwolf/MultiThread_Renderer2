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

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h> 

#include "primitives.h"


namespace SDLA {


  class Rendering {
    class Window;

    public:
    struct Position{
      WorldPos worldPos;
      ScreenPos screenPos;
    };

    struct SpriteInfo : public std::mutex{
      bool hidden = false;
      int angle = 0;
      Vec2* rotCenter = nullptr;
      Bounds area;

      Position pos;

      protected:
      SDL_RendererFlip flip = SDL_FLIP_NONE;
    };

    Rendering(){
      std::thread renderThread (mane, new Windows());
    } 

    ~Rendering(){
      renderThread.detach();
      // Destroy everything 
    }

    Window* newWindow(int layercount, Box windowSize, Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}, std::string name = "Abstraction", SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE);
    void closeWindow(Window* window);

    class Renderable{
      public:
      Renderable(std::string fileName, SpriteInfo* info = new SpriteInfo());
      void removeRenderable(Renderable* renderable);
      ~Renderable();


      protected:
      void updateSDLRect();
      // void render();
      std::atomic<bool> pendingErase = false;
      std::string fileName;
      SpriteInfo* info;

      private:
      SDL_Texture* sdlTexture;
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

    protected:
    struct Offset : public std::mutex{
      int x = 0, y = 0;
    };

    typedef struct {
      SpriteInfo* info;
      std::atomic<bool> pendingErase = false;
      std::vector<std::shared_ptr<Renderable>> renderables;
    } RenderableGroup;

    struct Layer {
      Offset offset;
      std::vector<std::shared_ptr<RenderableGroup>> renderableGroups;

      std::atomic<bool> hidden = false;
      std::atomic<bool> pendingErase = false;

    };

    struct Windows{
      std::vector<std::thread> members;
    };

    class Window {
      public:
      Window(
        int layerCount,
        Box windowSize,
        std::string name = "Abstraction",
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE
        )
      : layers(layerCount, std::shared_ptr<Layer>()){
        // Windows::members.push(this);
      }


      // Inform thread these should be deleted after next render
      void addImageGroup(int layer, std::shared_ptr<RenderableGroup> group);

      // Add to group 0
      void addImage(int layer, std::shared_ptr<Renderable> image);
      void removeLayer(int layer);
      void hideLayer(int layer);
      void showLayer(int layer);
      void offsetLayer(ScreenPos offset);

      SDL_Renderer* const getRenderer(){return sdlRenderer;};

      void display(){
        Renderable r = Renderable("fff");
        r.updateSDLRect();
      }
      void close();
      std::atomic<bool> pendingErase = false; 
      ~Window();

      protected:
      Box size;
      std::vector<std::shared_ptr<Layer>> layers;

      std::atomic<bool> mouseFocus;
      std::atomic<bool> keyboardFocus;
      std::atomic<bool> fullScreen;
      std::atomic<bool> minimized;
      std::atomic<bool> shown;



      private:
      SDL_Window* sdlWindow;
      SDL_Renderer* sdlRenderer;
      int windowID;

    };
      // std::atomic<std::vector<Window*>> bob;


    private:
    std::thread renderThread;
    // bool isRunning = false;

    // Insert new renderables at beginning of vector
    // compute layers backwards internally
    
    int mane(Windows w){
      while (true) {
        w.members.erase(
          std::remove_if(std::begin(w.members), std::end(w.members),
            [&] (Window* win) -> bool {
              if (win->pendingErase) {
                delete win;
                return true;
              }
              win->display();
              return false;
            }
           ),
           std::end(w.members)
        );
      }
    }
  };

}
#endif