#ifndef _MTR_BUFFER_h_
#define _MTR_BUFFER_h_

#include <vector>
#include <map>
#include <algorithm>


#include "renderUpdates.h"

namespace MTR{

  class Buffer{
    public:
    // Game loop moves the contents of write to swap during a lock using a simple = (then deleting the contents of write)
    // Render loop moves the content of swap to read during a lock using moveBuffer
    // Therefore (write -> [swap) -> read]
    RenderUpdates writeBuffer;
    RenderUpdates readBuffer ;
    RenderUpdates swapBuffer ;

    Buffer(int layers){
      writeBuffer = RenderUpdates(layers);
      readBuffer  = RenderUpdates(layers);
      swapBuffer  = RenderUpdates(layers);
    }

    Buffer(){};

    // Incomplete(SpriteGroup) or complete(SuperGroup/layer) deepCopy from source to target, deleting the contents of source
    // This is used during a mutex lock on the display thread loop, before each call to "display()"
    static void moveBuffer(RenderUpdates* source, RenderUpdates* target);
    static void cleanBuffer(RenderUpdates* buffer);
  };
}

#endif