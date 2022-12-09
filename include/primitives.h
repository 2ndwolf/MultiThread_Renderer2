#ifndef _MTR_PRIMITIVES_h_
#define _MTR_PRIMITIVES_h_

namespace MTR {

  typedef int ID;

}
  // typedef struct idInf{
  //   ID id;
  //   int layer;
  // } idInf;

  struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
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