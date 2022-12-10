#ifndef _MTR_WINDOWS_h
#define _MTR_WINDOWS_h

#include <thread>
#include <algorithm>
#include <map>
#include <string>
#include <atomic>

#include <SDL.h>

#include "buffer.h"
#include "primitives.h"

namespace MTR{

  class Window{
    private:
    ~Window(){
      // delete context;
      // delete window;
    }

    std::string name;
    Bounds bounds;

    std::thread renderThread;
    inline static std::thread multiThread;
    inline static bool multiInitialized = false;
    bool hasOwnThread = false;
    int ms = 0;
    int fps = 0;
    bool fpsShown;

    SDL_WindowFlags windowFlags; //?

    bool mouseFocus;
    bool keyboardFocus;
    bool fullScreen;
    bool minimized;
    bool shown;
    SDL_Window* window;
    SDL_Renderer* context;


    static void openMulti();
    static void closeMulti();

    void mane();
    void multiMane();

    protected:
    inline static std::map<std::string, Window*> windows = std::map<std::string, Window*>();
    inline static std::string currentWindow;

    Window(
      int layerCount,
      Box windowSize,
      std::string name,
      bool hasOwnThread = false,
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      SDL_WindowFlags mode = (SDL_WindowFlags) 0
    );

    public:
    std::mutex mutex;
    static std::mutex multimutex;
    Vec2 camPos = {0,0};
    MTR::Buffer buffer;
    void display();
    void close(){pendingErase = true;};

    bool pendingErase = false; 


    static std::string newWindow(
      int layerCount,
      Box windowSize,
      std::string name,
      bool hasOwnThread = false,
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}
      // SDL_WindowFlags mode = (SDL_WindowFlags) 0
    );

    int clampLayerIndex(int layer){
      return std::max(std::min(layer,getLayerCount()-1),0);
    };

    bool layerInBounds(int layer){
      return layer > 0 && layer < buffer.readBuffer.updLayer->upd.size();
    };

    const int getLayerCount(){return buffer.readBuffer.updLayer->upd.size();};
    static const int getLayerCount(std::string windowName){return windows[windowName]->buffer.readBuffer.updLayer->upd.size();};
    const bool getHasOwnThread() {return hasOwnThread;};

    SDL_Renderer* getContext(){return context;}

    static const std::string getCurrentWinName(){return currentWindow;};
    // static MTR::Window* getCurrentWindow(){return windows[currentWindow];};
    static MTR::Window* getWindow(std::string windowName){return windows[windowName];};
    static const int getWindowCount(){return windows.size();};

    static void updateAll();
    static void updateOne(std::string window);

  };

}



#endif