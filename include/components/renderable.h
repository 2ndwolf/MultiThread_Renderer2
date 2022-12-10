#ifndef _MTR_RENDERABLE_h_
#define _MTR_RENDERABLE_h_

#include <atomic>
#include <typeinfo>
#include <vector>
#include <string>

#include "primitives.h"
#include "fkenum.h"
// #include "window.h"


namespace MTR{

  namespace RND{
    class SuperGroup;

    class Renderable{
      protected:
      std::atomic<bool> updateAsRenderable;
      bool isQueued = false;

      bool checkSanity();
      void refreshRotCenter();

      public:
      bool addToQueue();
      void group(SuperGroup* supG);


      std::vector<std::string> windows = {};
      Bounds     bounds       = {INT_MIN, INT_MIN, 0, 0};
      // bool       ignoreCamera = true              ;
      bool         hidden       = false             ;
      int          angle        = 0                 ;
      Vec2         rotCenter    = {INT_MAX, INT_MAX};
      Vec2         zoomCenter   = {INT_MAX, INT_MAX};
      Vec2         zoom         = {100,100}         ;
      ENUM::Flip   flip         = ENUM::Flip::NONE  ;
      bool         angleFrSuper = true              ;
      SuperGroup*  ownerGroup   = nullptr           ;
      // void*      self         = nullptr           ;
      // int          layer        = 0                 ;

      // std::atomic<bool> toUpdate      = true;
      bool pendingErase = false;

      inline Renderable(
        std::vector<std::string> pwindows = {},
        Bounds     pbounds       = {INT_MIN, INT_MIN, 0, 0},
        // bool       pignoreCamera = true              ,
        bool         phidden       = false             ,
        int          pangle        = 0                 ,
        Vec2         protCenter    = {INT_MAX, INT_MAX},
        Vec2         pzoomCenter   = {INT_MAX, INT_MAX},
        Vec2         pzoom         = {100,100}         ,
        ENUM::Flip   pflip         = ENUM::Flip::NONE  ,
        bool         pangleFrSuper = true              ,
        SuperGroup*  pownerGroup   = nullptr       
        // int          player        = 0

      ) : windows     (pwindows     ),
          bounds      (pbounds      ),
          // ignoreCamera(pignoreCamera),
          hidden      (phidden      ),
          angle       (pangle       ),
          rotCenter   (protCenter   ),
          zoomCenter  (pzoomCenter  ),
          zoom        (pzoom        ),
          flip        (pflip        ),
          angleFrSuper(pangleFrSuper),
          ownerGroup  (pownerGroup  )
          // layer       (player       )/* , */
          // self        ((void*)&this )
      {};

      //   ignoreCamera = setting;
      //   setUpdate();
      // };

      // inline void setHidden      (bool setting){
      //   hidden = setting;
      //   setUpdate();
      // };

      // // Call from ORE::Renderable if Renderable
      // inline void setBounds      (Bounds newValue){
      //   bounds = newValue;
      //   setUpdate();
      // }; 

      // inline void setAngle       (int  newValue){
      //   angle = newValue;
      //   setUpdate();
      // };

      // inline void setRotCenter   (Vec2 newValue){
      //   // if(
      //   //   (newValue.x < 0 || newValue.x - INT_MAX < 0) &&
      //   //   (newValue.y < 0 || newValue.y - INT_MAX < 0)
      //   //   ){

      //     rotCenter = newValue;
      //   // } else rotCenter = {0,0};
      //   setUpdate();
      // };

      // inline void setZoomCenter  (Vec2 newValue){
      //   zoomCenter= newValue;
      //   setUpdate();
      // };

      // inline void setZoom        (float newValue){
      //   zoom = newValue;
      //   setUpdate();
      // };

      // inline void setAngleFrSuper(bool setting){
      //   angleFrSuper = setting;
      //   setUpdate();
      // };

      // inline void setUpdate(){
      //   // if(typeid(this) == typeid(Renderable))
      //   // if(copied) writeCycles=!writeCycles;
      //   toUpdate = true;
      // };

      public:
      // bool writeCycles;

      static void deepCopy(Renderable* source, Renderable* target);

      // Renderable& operator=(const Renderable& r){
      //   bounds       = r.bounds       ;
      //   ignoreCamera = r.ignoreCamera ;
      //   hidden       = r.hidden       ;
      //   angle        = r.angle        ;
      //   rotCenter    = r.rotCenter    ;
      //   zoomCenter   = r.zoomCenter   ;
      //   zoom         = r.zoom         ;
      //   flip         = r.flip         ;
      //   angleFrSuper = r.angleFrSuper ;
      //   return *this;
      // }
   };

  }
}

#endif