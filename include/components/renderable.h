#ifndef _MTR_RENDERABLE_h_
#define _MTR_RENDERABLE_h_

#include <atomic>
#include <typeinfo>
#include <vector>
#include <string>
#include <utility>

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
      void refreshRotCenter(SuperGroup* supG);

      public:
      bool addToQueue();
      void subscribeTo(SuperGroup* supG);


      // std::string window;
      std::vector<std::string> windows;
      Bounds     bounds       = {0, 0, 0, 0};
      // bool       ignoreCamera = true              ;
      bool         hidden       = false             ;
      int          angle        = 0                 ;
      Vec2         rotCenter    = {INT_MAX, INT_MAX};
      Vec2         zoomCenter   = {INT_MAX, INT_MAX};
      Vec2         zoom         = {100,100}         ;
      ENUM::Flip   flip         = ENUM::Flip::NONE  ;
      bool         angleFrSuper = true              ;
      void*        ownerGroup   = nullptr           ;
      void*        self         = nullptr           ;
      // int          layer        = 0                 ;

      // std::atomic<bool> toUpdate      = true;
      bool pendingErase = false;

      Renderable(
        std::vector<std::string>  pwindows = {},
        Bounds       pbounds       = {0, 0, 0, 0},
        // bool       pignoreCamera = true              ,
        bool         phidden       = false             ,
        int          pangle        = 0                 ,
        Vec2         protCenter    = {INT_MAX, INT_MAX},
        Vec2         pzoomCenter   = {INT_MAX, INT_MAX},
        Vec2         pzoom         = {100,100}         ,
        ENUM::Flip   pflip         = ENUM::Flip::NONE  ,
        bool         pangleFrSuper = true              ,
        void*        pownerGroup   = nullptr       
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
          ownerGroup  (pownerGroup  ),
          // layer       (player       )/* , */
          self        ((void*)this  )
      {};

      public:

      // static void deepCopy(Renderable* source, Renderable* target);

      Renderable(const Renderable& old_obj){
        bounds       = old_obj.bounds       ;
        hidden       = old_obj.hidden       ;
        angle        = old_obj.angle        ;
        rotCenter    = old_obj.rotCenter    ;
        zoomCenter   = old_obj.zoomCenter   ;
        zoom         = old_obj.zoom         ;
        flip         = old_obj.flip         ;
        angleFrSuper = old_obj.angleFrSuper ;
        ownerGroup   = old_obj.ownerGroup   ;
        self         = old_obj.self         ;
      }

      Renderable(Renderable&& other){
        bounds       = other.bounds       ;
        hidden       = other.hidden       ;
        angle        = other.angle        ;
        rotCenter    = other.rotCenter    ;
        zoomCenter   = other.zoomCenter   ;
        zoom         = other.zoom         ;
        flip         = other.flip         ;
        angleFrSuper = other.angleFrSuper ;
        ownerGroup   = other.ownerGroup   ;
        self         = other.self         ;

        other.bounds       = {0,0,0,0};
        other.hidden       = false;
        other.angle        = 0;
        other.rotCenter    = {0,0};
        other.zoomCenter   = {0,0};
        other.zoom         = {0,0};
        other.flip         = ENUM::Flip::NONE;
        other.angleFrSuper = false;
        other.ownerGroup   = nullptr;
        other.self         = nullptr;
      }

      Renderable& operator=(const Renderable& other){
        if(this != &other){
          Renderable(other).swap(*this);
        }
        return *this;
      }

      void swap(Renderable& r) noexcept {
        std::swap(this->bounds      , r.bounds      );
        std::swap(this->hidden      , r.hidden      );
        std::swap(this->angle       , r.angle       );
        std::swap(this->rotCenter   , r.rotCenter   );
        std::swap(this->zoomCenter  , r.zoomCenter  );
        std::swap(this->zoom        , r.zoom        );
        std::swap(this->flip        , r.flip        );
        std::swap(this->angleFrSuper, r.angleFrSuper);
        std::swap(this->ownerGroup  , r.ownerGroup  );
        std::swap(this->self        , r.self        );
      }

      Renderable& operator=(Renderable&& other){
        if(this != &other){
          bounds       = other.bounds       ;
          hidden       = other.hidden       ;
          angle        = other.angle        ;
          rotCenter    = other.rotCenter    ;
          zoomCenter   = other.zoomCenter   ;
          zoom         = other.zoom         ;
          flip         = other.flip         ;
          angleFrSuper = other.angleFrSuper ;
          ownerGroup   = other.ownerGroup   ;
          self         = other.self         ;

          other.bounds       = {0,0,0,0};
          other.hidden       = false;
          other.angle        = 0;
          other.rotCenter    = {0,0};
          other.zoomCenter   = {0,0};
          other.zoom         = {0,0};
          other.flip         = ENUM::Flip::NONE;
          other.angleFrSuper = false;
          other.ownerGroup   = nullptr;
          other.self         = nullptr;
        }
        return *this;
      }
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