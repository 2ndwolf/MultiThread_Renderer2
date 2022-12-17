#ifndef _MTR_LAYER_h_
#define _MTR_LAYER_h_

#include <vector>
#include <string>
#include <utility>


#include "primitives.h"
// #include "window.h"
#include "renderable.h"

namespace MTR{
  namespace RND{

    class Layer : public Renderable {

      public:
      bool isDue = true;


      Layer(const Layer& old_obj) : Renderable(old_obj){}
      Layer(Layer&& other) : Renderable(other){}
      Layer(bool pisDue):isDue(pisDue){
        // bounds.pos.x = 0;
        // bounds.pos.y = 0;
      }

      Layer& operator=(Layer&& other){
        Renderable::operator=(other);
        isDue = false;
        return *this;
      }

      Layer& operator=(const Layer& other){
        if(this!=&other){
          Layer(other).swap(*this);
          // return tmp;
        }
        
        return *this;
      }

      // inline Layer(){};

      // int layerNumber = 0;
      // std::vector<std::string> windows;
      // Vec2 offset = {0,0};
      // bool hidden = false;

      // inline ~Layer(){};

      void update(Layer* layer);
    };
  }
}

#endif