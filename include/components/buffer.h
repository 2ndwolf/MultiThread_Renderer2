#ifndef _MTR_BUFFER_h_
#define _MTR_BUFFER_h_

#include <vector>
#include <map>
#include <algorithm>


#include "renderUpdates.h"

namespace MTR{

  class Buffer{
    public:
    RenderUpdates writeBuffer;
    RenderUpdates readBuffer ;
    RenderUpdates swapBuffer ;

    Buffer(int layers){
      writeBuffer = RenderUpdates(layers);
      readBuffer  = RenderUpdates(layers);
      swapBuffer  = RenderUpdates(layers);
    }

    Buffer(){};

    static void moveBuffer(RenderUpdates* source, RenderUpdates* target);
  };
}

#endif