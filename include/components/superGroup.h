#ifndef _MTR_SUPERGROUP_h_
#define _MTR_SUPERGROUP_h_

#include <vector>
#include <utility>

#include "renderable.h"
// #include "info.h"
#include "primitives.h"
// #include "window.h"

namespace MTR{

  namespace RND{

    class SuperGroup : public Renderable {
      private:
        int useCount      = 0;
        std::vector<SuperGroup*> childs = std::vector<SuperGroup*>();

      public:

      SuperGroup(
        std::vector<std::string> pwindows
        ) :
        Renderable(pwindows)
      {};

      SuperGroup(const SuperGroup&  old_obj) : Renderable(old_obj){}
      SuperGroup(SuperGroup&& old_obj) : Renderable(old_obj){}

      SuperGroup& operator=(const SuperGroup& other){
        if(this!=&other){
          SuperGroup(other).swap(*this);
        }
        return *this;
      }

      // void swap(SuperGroup& supG){

      // }

      SuperGroup& operator=(SuperGroup&& other){
        if(this != &other){
          // SuperGroup tmp(other);
          // std::swap(*this,tmp);
          Renderable::operator=(other);
        }
        return *this;
      }

      inline static std::vector<SuperGroup*> pool = std::vector<SuperGroup*>();

      static void update(std::vector<SuperGroup*> groups);

      static void deepCopy(SuperGroup* source, SuperGroup* target);
      bool checkSanity();

      void updateBounds(Renderable* rend);
      void refreshBounds();

      int groupSuper(std::vector<SuperGroup*> spGs, bool setCenter = true);

      void unGroup(std::vector<SuperGroup*> spGs);

    };


  }
}

#endif