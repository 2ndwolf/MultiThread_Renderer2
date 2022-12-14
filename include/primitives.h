#ifndef _MTR_PRIMITIVES_h_
#define _MTR_PRIMITIVES_h_

#include <cstdint>

namespace MTR {

  typedef int ID;

}

  struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };

  typedef struct Vec2{
    int x = 0, y = 0;

    bool operator==(const Vec2& comp){
      return x == comp.x && y == comp.y;
    }

    // Vec2 operator-(const Vec2& comp){
    //   return {x-comp.x, y-comp.y};
    // }

    // Vec2 operator+(const Vec2& comp){
    //   return {x+comp.x, y+comp.y};
    // }

    // Vec2 operator/(const Vec2 comp){
    //   return {x/comp.x, y/comp.y};
    // }

  } Vec2;

  typedef struct Box{
    int width = 0, height = 0;

    bool operator==(const Box& comp){
      return width == comp.width && height == comp.height;
    }
    
  } Box;

  typedef struct Bounds{
    Vec2 pos;
    Box box;

    bool operator==(const Bounds& comp){
      return box == comp.box && pos == comp.pos;
    } 

  } Bounds;
  
#endif