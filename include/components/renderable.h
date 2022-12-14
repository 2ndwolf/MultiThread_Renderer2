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
      std::atomic<bool> updateAsRenderable = false;
      bool isQueued = false;

      bool checkSanity();
      void refreshRotCenter();

      public:
      bool addToQueue();
      void group(SuperGroup* supG);


      // std::string window;
      std::vector<std::string> windows;
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

      Renderable(
        std::vector<std::string>  pwindows,
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

      public:

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