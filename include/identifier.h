#ifndef _MTR_IDENTIFIER_h_
#define _MTR_IDENTIFIER_h_

#include "primitives.h"

namespace MTR{

  class Identifier{
    private:
      // static MTR::ID currtID;
      static MTR::ID textID;
    public:
      // static MTR::ID newID(){
      //   currtID++;
      //   return currtID;
      // }
      inline static MTR::ID newtextID(){
        textID++;
        return textID;
      };
      inline static MTR::ID noID(){
        return -1;
      };
  };

}

#endif