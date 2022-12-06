#ifndef _FK_information_h_
#define _FK_information_h_

#include <atomic>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <cstdint>
#include <mutex>

#include <SDL.h>
#include <SDL_image.h> 
#include <SDL_ttf.h>

#include "fkore.h"

#include "TSRPrimitives.h"
#include "fkenum.h"
// #include "renderable.h"


namespace FK {
  namespace AT{
    struct SpriteGroup;
  }
  namespace ORE {
    inline std::string placeHolderImage = "assets/gen_specialchest.png";
    inline const std::string GAMENAME   = "i!i!i!i!i!i!i!i!i!i!i!i!_ZEMOSTFUN_GAEM_EVAH_i!i!i!i!i!i!i!i!i!i!i!i!";

    class Renderable;
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


              // it->second->display(it->second->name);


              
            }
            it++;
          }
          if(otherThread == windows.size()) closeMulti();
          lastLoopTime = events.lastPoll;
        }
      }
    };


    void mane();

    bool hasOwnThread = false;

    int ms = 0;
    int fps = 0;
    bool fpsShown;

    SDL_WindowFlags windowFlags; //?

    struct Layer{
      inline Layer(){};

      std::vector<std::shared_ptr<AT::SpriteGroup>> groups;

      // FK::ID id;
      Vec2 offset = {0,0};
      bool hidden = false;
    };

    std::vector<Layer> readBuffer  = std::vector<Layer>();
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
    inline static std::mutex multiMutex;
    std::mutex mutex;
    // std::mutex spriteMutex;
    // std::mutex groupMutex;
    // std::mutex windowMutex;

    Vec2 camPos = {0,0};

    int clampLayerIndex(int layer){
      return std::max(std::min(layer,getLayerCount()-1),0);
    };

    bool layerInBounds(int layer){
      return layer > 0 && layer < writeBuffer.size();
    };

    const int getBufferSize(){return writeBuffer.size();};

    static std::string newWindow(
      int layerCount = 5,
      Box windowSize = {500,500},
      std::string name = ORE::GAMENAME,
      bool hasOwnThread = false,
      Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
      SDL_WindowFlags mode = (SDL_WindowFlags) 0
    );
    Window(
      int layerCount   = 5,
      Box windowSize   = {500,500},
      std::string name = ORE::GAMENAME,
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

    bool pendingErase = false; 

    void display(std::string window, ORE::Renderable rend);
    void close(){pendingErase = true;};

  };

  namespace AT  {
    // class Renderable;
    // class Information;
    // class SuperGroup;
    // class SpriteGroup;
    // class SpriteInformation;
    // class TextInformation;

    // TODO :: Replace Vec2 with "bounds"
    // TODO :: Remove hard coded 50 and 10 values
    class Information{
      public:
      inline Information(){};
      // Offset was replaced with "bounds"
      // Vec2 offset = {0,0};
      // Bounds bounds = {NULL,NULL,NULL,NULL};
      Bounds bounds = {50,50,50,50};
      bool ignoreCamera = true;
      bool hidden = false;
      int angle = 0;
      Vec2* rotCenter = nullptr;
      
      // BoxPTR boxPtr = {0,0};

      std::atomic<bool> pendingErase = false;
      std::string myWindow = "uninitialized";

      // PLZ PLZ AT LEAST fill myWindow
      inline Information(
        // Vec2 offset = Vec2{0,0},
        std::string myWindow,
        Bounds bounds = {0,0,10,10},
        bool ignoreCamera = true,
        bool hidden = false,
        int angle = 0,
        Vec2* rotCenter = nullptr
      ){
        this->bounds = bounds;
        this->ignoreCamera = ignoreCamera;
        this->hidden = hidden;
        this->angle = angle;
        this->rotCenter = rotCenter;
        this->myWindow = myWindow;
      };

      // Y U NO FILL myWindow X(
      inline Information(
        Bounds bounds,
        bool ignoreCamera = true,
        bool hidden = false,
        int angle = 0,
        Vec2* rotCenter = nullptr
      ){
        this->bounds = bounds;
        this->ignoreCamera = ignoreCamera;
        this->hidden = hidden;
        this->angle = angle;
        this->rotCenter = rotCenter;
        // this->myWindow = myWindow;
      };

      inline void fillMe(Information* child);
    };
    void Information::fillMe(Information* child){
      child->bounds        = this->bounds      ;
      child->ignoreCamera  = this->ignoreCamera;
      child->hidden        = this->hidden;
      child->angle         = this->angle;
      child->rotCenter     = this->rotCenter;
      child->rotCenter     = this->rotCenter;
      child->myWindow      = this->myWindow;
    };

    class SuperGroup : public Information {
      public:
      // inline SuperGroup();
      inline SuperGroup(std::shared_ptr<Information> information, bool isRotationCenter = false);
      // SuperGroup(Information information) : Information(information){};

      std::shared_ptr<SuperGroup> superGroup = nullptr;
      // std::shared_ptr<SuperGroup> childGroup = nullptr;
      bool isRotationCenter = false;
    };
    // SuperGroup::SuperGroup(){};
    SuperGroup::SuperGroup(std::shared_ptr<Information> information, bool isRotationCenter){
      information->fillMe(this);
      this->isRotationCenter = isRotationCenter;
    };


    class SpriteGroup : public Information {
      public:
      // inline SpriteGroup(){};
      inline SpriteGroup(std::shared_ptr<Information> information
      , bool isRotationCenter = false){
        information->fillMe(this);
        // this->isRotationCenter = isRotationCenter;
      };
      // SpriteGroup(Information information) : Information(information){};

      std::shared_ptr<SuperGroup> superGroup = nullptr;
      std::vector<std::shared_ptr<ORE::Renderable>> sprites = std::vector<std::shared_ptr<ORE::Renderable>>();

      Vec2 worldPos = {0,0};

    };
    // SpriteGroup::SpriteGroup(){};
    // SpriteGroup::SpriteGroup(std::shared_ptr<Information> information, bool isRotationCenter);


    class SpriteInformation : public Information {
      public:
      Bounds area = {0,0,0,0};
      std::string fileName = "assets/gen_specialchest.png";
      ENUM::Flip flip = ENUM::Flip::NONE;
      std::shared_ptr<SpriteGroup> ownerGroup;

      // inline SpriteInformation(){};

      inline SpriteInformation(std::shared_ptr<Information> information,
        Bounds pArea = {0,0,0,0},
        std::string pFileName = "assets/gen_specialchest.png",
        ENUM::Flip pFlip = ENUM::Flip::NONE
      ) : area(pArea), fileName(pFileName), flip(pFlip){
        information->fillMe(this);
      };

      // NAVAH MAEK THIS A POINTER OMG!
      class Busy{
        private:
        inline static bool alreadyBusy = false;
        public:
        bool hasOwnThread = false;
        std::string win = FK::Window::getCurrentWindowName();
        // std::shared_ptr<FK::ORE::Renderable> renderable;

        inline Busy(std::string win = FK::Window::getCurrentWindowName()){
          // while(alreadyBusy){}
          // alreadyBusy = true;
          this->win = win;
          hasOwnThread = FK::Window::getWindow(win)->getHasOwnThread();
          hasOwnThread ?
          FK::Window::getWindow(win)->mutex     .lock  () :
          FK::Window::getWindow(win)->multiMutex.lock  () ;
        }

        inline ~Busy(){
          hasOwnThread ?
          FK::Window::getWindow(win)->mutex     .unlock() :
          FK::Window::getWindow(win)->multiMutex.unlock() ;
          alreadyBusy = false;
        }
      };

      // RAWMEMBER TO !!DELETE!! THE!! POINTER!!
      inline SpriteInformation* copy(){

        FK::AT::SpriteInformation*  bob = ((FK::AT::SpriteInformation*)this);
        FK::AT::SpriteInformation*  che = new FK::AT::SpriteInformation(std::make_shared<Information>("Abstraction"));
         che->area         =  bob->area        ;
         che->flip         =  bob->flip        ;
         che->angle        =  bob->angle       ;
         che->bounds       =  bob->bounds      ;
         che->hidden       =  bob->hidden      ;
         che->myWindow     =  bob->myWindow    ;
         che->fileName     =  bob->fileName    ;
         che->rotCenter    =  new Vec2{0,0}    ;
         che->rotCenter    =  bob->rotCenter   ;
         che->ownerGroup   =  bob->ownerGroup  ;
         che->ignoreCamera =  bob->ignoreCamera;

        return che;
      }

        // inline auto isBusy()
        // {
        //   std::uniquelock<std::mutex> lck(lock);
        //   buildProjectionDataCpy = buildProjectionData;
        //   reqProjectionDataCpy = reqProjectionData;
        //   return requ
        // }

      // std::shared_ptr<SpriteInformation> share(){
      //   return std::shared_ptr<SpriteInformation>(this);
      // };
    };
    // SpriteInformation::SpriteInformation(){};
    // SpriteInformation::SpriteInformation(std::shared_ptr<Information> information,
    //     Bounds area,
    //     std::string fileName,
    //     ENUM::Flip flip
    //   ){
    //     information->fillMe(this);
    //   };

    // class SpriteSheet : public SpriteInformation {}


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
  namespace ORE {
    class Renderable {
      protected:
      bool getAngleFromGroup = true;
      

      public:
      class getSprInfoWrite{
        private:
        std::shared_ptr<ORE::Renderable> myRenderable;

        public:
        FK::AT::SpriteInformation::Busy busy = FK::AT::SpriteInformation::Busy();
        std::shared_ptr<FK::AT::SpriteInformation> sprInfo;

        inline getSprInfoWrite(std::shared_ptr<ORE::Renderable> renderable){
          this->myRenderable = renderable;
          sprInfo = renderable->sprInfoWrite;
        }

        inline ~getSprInfoWrite(){
          myRenderable->RefreshSpriteInfo();
        }
      };

      std::shared_ptr<FK::AT::SpriteInformation> sprInfoWrite;
      FK::AT::SpriteInformation* sprInfoRead;

      // class RefreshSpriteInfo{
      inline void RefreshSpriteInfo(){
        // delete sprInfoRead;
        sprInfoRead = sprInfoWrite->copy();
      }
      // };

      void changeSurface(std::string fileName);
      void setRotationCenter(Vec2 center);

      // inline FK::AT::SpriteInformation getInformation(){return *information;};
      // bool hidden = false;
      bool pendingErase = false;
      std::string myWindow;
      std::shared_ptr<FK::AT::SpriteGroup> ownerGroup;

      SDL_Texture* const getTexture(){return texture;};
      void setCrop(Bounds crop, bool rendererCall = false);
      bool getIgnoreCamera(){return ownerGroup->ignoreCamera;}

      Bounds getBounds();

      // information->flip = ENUM::Flip
      void setFlip(ENUM::Flip flip);


      // For display loop exclusively
      void const setTexture(SDL_Texture* tex){texture = tex;};

      // TODO :: WAS MOVED AROUND
      // static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);
      static FKORE::SDLSurface* loadSurface(std::string fileName, bool keepImgInMemory = false);



      // RenderTypes type;

      SDL_Rect* const getSRCRect(){return &srcRect;};
      SDL_Rect* const getSDLRect(){return &tgtRect;};
      // std::string const getFileName(){return spriteInformation->fileName;};
      // std::string const getFileName(){return spriteInformation->fileName;};

      // RAWMEMBER TO DELETE!!

      bool texQueued = true;

      inline Renderable(std::shared_ptr<FK::AT::SpriteInformation> information, int layer, std::string window, std::shared_ptr<FK::AT::SpriteGroup> ownerGroup){
        // this->ignoreCamera = ignoreCamera;
        // std::shared_ptr<SpriteInfo> sprI = std::make
        this->sprInfoWrite = information;
        this->sprInfoWrite->myWindow = window;
        RefreshSpriteInfo();

        this->layer = FK::Window::getWindow(window)->clampLayerIndex(layer);
        // this->info->layer = this->layer;
        this->myWindow = window;

        this->ownerGroup = ownerGroup;
        // this->fileName = info->fileName;
      };
      protected:
      inline Renderable(){};


      SDL_Rect tgtRect;
      SDL_Rect srcRect;
      // std::string fileName;
      // std::atomic<bool> ignoreCamera = true;
      int layer;
      inline static std::string placeHolder = placeHolderImage;


      private:

      private:
      SDL_Texture* texture;

    };

  }
}


#endif