#ifndef _SDLA_identifier_h
#define _SDLA_identifier_h

#include "primitives.h"

namespace SDLA{

  class Identifier{
    private:
      inline static SDLA::ID currtID = -1;
    public:
      static SDLA::ID newID(){
        currtID++;
        return currtID;
      }
      static SDLA::ID noID(){
        return -1;
      }
      // static void initIDs(){
      //   SDLA::ID currtID = -1;
      // }
  };

}

#endif