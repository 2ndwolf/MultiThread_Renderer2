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
    bool fpsShown = false;
    bool somethingToDraw = false;

    SDL_WindowFlags windowFlags; //?

    bool mouseFocus = false;
    bool keyboardFocus = false;
    bool fullScreen = false;
    bool minimized = false;
    bool shown = false;
    SDL_Window* window;


    static void openMulti();
    static void closeMulti();

    void mane();
    void multiMane();

    protected:
    inline static std::map<std::string, Window*> windows;
    inline static std::string currentWindow;

    Window(
      int layerCount,
      Box windowSize,
      const std::string& name,
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      bool hasOwnThread = false,
      SDL_WindowFlags mode = (SDL_WindowFlags) 0
    );

    public:
    std::mutex mutex;
    inline static std::mutex multimutex;
    Vec2 camPos = {0,0};
    MTR::Buffer buffer;
    void display();
    void close(){pendingErase = true;};

    bool pendingErase = false; 

    SDL_Renderer* context;

    static std::string newWindow(
      int layerCount,
      Box windowSize,
      const std::string& name,
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      bool hasOwnThread = false
      // SDL_WindowFlags mode = (SDL_WindowFlags) 0
    );

    int clampLayerIndex(int layer){
      return std::max(std::min(layer,getLayerCount()-1),0);
    };

    bool layerInBounds(int layer){
      return layer > 0 && layer < buffer.readBuffer.updLayer.size();
    };

    const int getLayerCount(){return buffer.readBuffer.updLayer.size();};
    static const int getLayerCount(const std::string& windowName){return windows[windowName]->buffer.readBuffer.updLayer.size();};
    const bool getHasOwnThread() {return hasOwnThread;};

    SDL_Renderer* getContext(){return context;}

    static const std::string getCurrentWinName(){return currentWindow;};
    // static MTR::Window* getCurrentWindow(){return windows[currentWindow];};
    static Window* getWindow(const std::string& windowName);
    // static std::map<std::string, Window*> getWindows();
    static const int getWindowCount(){return windows.size();};

    static void updateAll();
    static void updateOne(const std::string& window);

  };

}



#endif