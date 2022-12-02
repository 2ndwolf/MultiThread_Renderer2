#ifndef _FK_identifier_h
#define _FK_identifier_h

#include "primitives.h"

namespace FK{

  namespace ORE{

    class Identifier{
      private:
        inline static FK::ID currtID = -1;
        inline static FK::ID txtID = -1;
        inline static FK::ID winID = -1;
      public:
        static FK::ID newID(){
          currtID++;
          return currtID;
        }
        static FK::ID newtxtID(){
          txtID++;
          return txtID;
        }
        static FK::ID newWinID(){
          winID++;
          return winID;
        }
        static FK::ID noID(){
          return -1;
        }
        // static void initIDs(){
        //   FK::ID currtID = -1;
        // }
    };
  }

}

#endif