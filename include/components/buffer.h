#ifndef _MTR_BUFFER_h_
#define _MTR_BUFFER_h_

#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <utility>


#include "layer.h"
#include "image.h"
#include "text.h"
#include "spriteGroup.h"
#include "superGroup.h"
// #include "identifier.h"


namespace MTR{

  class DeferBuffer{
    // protected:
    // struct cmpr {
    //   // bool cmpr(void* a, void* b);
    //   bool operator()(void* a, void* b) const {
    //     return (intptr_t)a < (intptr_t)b;
    //   }
    // };

    public:
                std::          map<void*, MTR::RND::Layer           >  dfrLayer      ;
    std::vector<std::          map<void*, MTR::RND::SpriteGroup     >> dfrSpriteGroup;
                std::unordered_map<void*, MTR::RND::Image           >  dfrImage      ;
                std::unordered_map<void*, MTR::RND::SuperGroup      >  dfrSuperGroup ;

    DeferBuffer(int layers){
      dfrLayer       =             std::          map<void*, MTR::RND::Layer       > ();
      dfrSpriteGroup = std::vector<std::          map<void*, MTR::RND::SpriteGroup >>(layers);
      dfrImage       =             std::unordered_map<void*, MTR::RND::Image       > ();
      dfrSuperGroup  =             std::unordered_map<void*, MTR::RND::SuperGroup  > ();
    };

    DeferBuffer(){};

    DeferBuffer& operator<<(DeferBuffer&& old_obj);

    ~DeferBuffer(){}
    // DeferBuffer& operator=(DeferBuffer&& other){
    //   if(this != &other){
    //     dfrLayer       = other.dfrLayer      ;
    //     dfrSpriteGroup = other.dfrSpriteGroup;
    //     dfrSuperGroup  = other.dfrSuperGroup ;
    //   }
    //   return *this;
    // }

    // DeferBuffer operator=(DeferBuffer&& other){
    //   DeferBuffer dfB = DeferBuffer(**other);
    //   std::swap(*this, dfB);
    //   return *this;
    // }
  };

  class WriteBuffer : public DeferBuffer{


    public:
                std::          map<void*, MTR::RND::Layer      *      >  ptrLayer      ;
    std::vector<std::          map<void*, MTR::RND::SpriteGroup*      >> ptrSpriteGroup;
                std::          map<void*, MTR::RND::Image      *      >  ptrImage      ;
                std::          map<void*, MTR::RND::SuperGroup *      >  ptrSuperGroup ;

    WriteBuffer(int layers) : DeferBuffer(layers){
      ptrLayer       =             std::          map<void*, MTR::RND::Layer      * > ();
      ptrSpriteGroup = std::vector<std::          map<void*, MTR::RND::SpriteGroup* >>(layers);
      ptrImage       =             std::          map<void*, MTR::RND::Image      * > ();
      ptrSuperGroup  =             std::          map<void*, MTR::RND::SuperGroup * > ();
    };

    WriteBuffer(){};

    // WriteBuffer(const WriteBuffer& old_obj) : DeferBuffer((DeferBuffer&)old_obj){};

  };

  class RenderUpdates{
    public:
    enum updType{
      LAYER,
      IMAGE,
      SPRITEGROUP,
      SUPERGROUP,
      LENGTH
    };
  };


  class Buffer{
    public:
    // Game loop moves the contents of write to swap during a lock using a simple = (then deleting the contents of write)
    // Render loop moves the content of swap to read during a lock using moveBuffer
    // Therefore (write -> [swap) -> read]
    WriteBuffer writeBuffer; // Contains only updates
    // DeferBuffer swapBuffer ; // Contains only updates
    DeferBuffer readBuffer; // Contains all that is drawn

    Buffer(int layers){
      writeBuffer = WriteBuffer(layers);
      readBuffer  = DeferBuffer(layers);
      // swapBuffer  = DeferBuffer(layers);
    }

    Buffer(){};

    // Incomplete(SpriteGroup) or complete(SuperGroup/layer) deepCopy from source to target, deleting the contents of source
    // This is used during a mutex lock on the display thread loop, before each call to "display()"
    static void addUpdates(WriteBuffer* source, DeferBuffer* target);
    // static void cleanBuffer(WriteBuffer* buffer);
  };
}

#endif