#ifndef _FK_WINDOW_h
#define _FK_WINDOW_h

#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <map>

#include <SDL.h>

#include "information.h"
#include "primitives.h"

namespace FK{
  namespace AT{
    // static std::string newWindow(
    //     int layerCount,
    //     Box windowSize,
    //     std::string name = "Abstraction",
    //     Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
    //     SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE,
    //     bool hasOwnThread = false
    // );
    // static void setGroupAsRotationCenter(std::shared_ptr<FK::AT::SpriteGroup> sG);
    // static void setGroupRotationCenter(std::shared_ptr<FK::AT::SpriteGroup> sG, Vec2 center);
    // // static const std::string getCurrentWindow(){return currentWindow;};
    // static const std::string getCurrentWindowName(){return FKORE::currentWindow;};
    // // static const std::map<std::string, std::shared_ptr<Window>>
    // static std::shared_ptr<FK::Window> getCurrentWindow(){return FKORE::windows[FKORE::currentWindow];};
    // static std::shared_ptr<FK::Window> getWindow(std::string windowName){return FKORE::windows[windowName];};
    // static const int getWindowCount(){return FKORE::windows.size();};
  }

  class Window {
      protected:
      inline static std::map<std::string, std::shared_ptr<Window>> windows = std::map<std::string, std::shared_ptr<Window>>();
      inline static std::string currentWindow;
      inline static bool multiInitialized = false;
      // inline Window(){};

      Vec2 position;

      static void openMulti(){
        if(!multiInitialized) {
          multiInitialized = true;
          std::thread(multiMane).detach();
        }
      }

      static void closeMulti(){
        multiInitialized = false;
      }

      struct SDLEvent{SDL_Event event; int lastPoll;};
      inline static SDLEvent events;
      void pollEvents();

      private:
      // inline static std::thread multiThread;
      // inline static Window* threadWindow;

      // MultiWindow Thread
      // std::thread multiWindowThread;
      inline static void multiMane(){
        std::map<std::string, std::shared_ptr<Window>>::iterator it;
        int lastLoopTime = 0;
        int lastPollTime = 0;
        // bool breakOutOfWhile = false;
        while (multiInitialized) {
          if(windows.size() != 0) {
            int otherThread = 0;
            for(it = windows.begin(); it != windows.end(); it++){

              if (it->second == nullptr ||
                  it->second->pendingErase) {

                it = windows.erase(it);
                if(windows.size() == 0) {
                  closeMulti();
                }
                continue;

              } else if (it->second->getHasOwnThread()) otherThread++;
              else {
                if(lastLoopTime < events.lastPoll || lastPollTime < events.lastPoll){
                  lastPollTime = events.lastPoll;
                  it->second->pollEvents();
                }
                it->second->display();
              }
              it++;
            }
            if(otherThread == windows.size()) closeMulti();
            lastLoopTime = events.lastPoll;
          }
        }
      };


      inline void mane(){
        int lastEventTime;
        while(!pendingErase) {
          if(events.lastPoll != lastEventTime){
            lastEventTime = events.lastPoll;
            pollEvents();
          }
          display();
        }
        windows[name] = nullptr;
      }

      bool hasOwnThread = false;

      int ms = 0;
      int fps = 0;
      bool fpsShown;

      SDL_WindowFlags windowFlags; //?

      struct Layer{
        inline Layer(){};

        std::vector<std::shared_ptr<AT::SpriteGroup>> groups = std::vector<std::shared_ptr<AT::SpriteGroup>>();

        FK::ID id;
        Vec2 offset = {0,0};
        bool hidden = false;
      };

      std::vector<Layer> readBuffer = std::vector<Layer>();
      std::vector<Layer> writeBuffer = std::vector<Layer>();

      std::atomic<bool> mouseFocus;
      std::atomic<bool> keyboardFocus;
      std::atomic<bool> fullScreen;
      std::atomic<bool> minimized;
      std::atomic<bool> shown;
      SDL_Window* window;
      SDL_Renderer* context;
      std::string name;
      Box size;



      void updateGroupBuffer(){
        // readBuffer = {};
        // mutex.lock();
        // for(int i = 0; i < writeBuffer.size(); i++){
        //   readBuffer.push_back(writeBuffer[i]);
        // }
        // mutex.unlock();
        readBuffer = writeBuffer;
      };

      public:
      std::mutex mutex;
 
      Vec2 camPos = {0,0};

      int clampLayerIndex(int layer){
        return std::max(std::min(layer,getLayerCount()-1),0);
      };

      bool layerInBounds(int layer){
        return layer > 0 && layer < writeBuffer.size();
      };

      const int getBufferSize(){return writeBuffer.size();};

      static std::string newWindow(
        int layerCount,
        Box windowSize,
        std::string name,
        bool hasOwnThread = false,
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = (SDL_WindowFlags) 0
      );
      Window(
        int layerCount,
        Box windowSize,
        std::string name,
        bool hasOwnThread = false,
        Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
        SDL_WindowFlags mode = (SDL_WindowFlags) 0
        );



      //Layer methods
      void hideLayer(int layer){if(layerInBounds(layer)) writeBuffer[layer].hidden = true;};
      void showLayer(int layer){if(layerInBounds(layer)) writeBuffer[layer].hidden = false;};
      void offsetLayer(int layer, Vec2 offset){if(layerInBounds(layer)) writeBuffer[layer].offset = offset;};

      
      std::vector<Layer> getReadBuffer(){return readBuffer;};
      std::vector<Layer> getBuffer(){return writeBuffer;};
      const bool getHasOwnThread() {return hasOwnThread;};

      // Getters
      int getLayerCount(){return readBuffer.size();};
      Layer* getLayer(int layer){return &readBuffer[clampLayerIndex(layer)];};

      static const std::string getCurrentWindowName(){return currentWindow;};
      static std::shared_ptr<FK::Window> getCurrentWindow(){return windows[currentWindow];};
      static std::shared_ptr<FK::Window> getWindow(std::string windowName){return windows[windowName];};
      static const int getWindowCount(){return windows.size();};

      std::string getName(){return name;};

      static void updateEvents(SDL_Event event){
        events.event = event;
        events.lastPoll = SDL_GetTicks();
        // std::map<std::string, std::shared_ptr<Window>>::iterator it;
        // if(windows.size() != 0) {/* windows[currentWindow]->display(); */
        //   for(it = windows.begin(); it != windows.end(); it++){
        //     it->second->pollEvents(event);
        //   }
        // }
      }

      std::atomic<bool> pendingErase = false; 

      void display();
      void close(){pendingErase = true;};

    };

    // class MultiWindow : private Window {
    //   public:
    //   inline MultiWindow(){
    //   };

    //   private:
    // }
}

#endif