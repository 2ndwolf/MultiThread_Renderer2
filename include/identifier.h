#ifndef _SDLA_identifier_h
#define _SDLA_identifier_h

#include "primitives.h"

namespace SDLA{

  class Identifier{
    private:
      static SDLA::ID currtID;
    public:
      static SDLA::ID newID(){
        currtID++;
        return currtID;
      }
      static SDLA::ID noID(){
        return -1;
      }
  };

}

#endif