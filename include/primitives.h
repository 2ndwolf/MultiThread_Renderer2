#ifndef _SDLA_primitives_h
#define _SDLA_primitives_h

namespace SDLA {

  typedef int ID;

  typedef struct idInf{
    ID id;
    int layer;
  } idInf;

  typedef struct Vec2{
    int x = 0, y = 0;

    bool operator==(const Vec2& comp){
      return x == comp.x && y == comp.y;
    }

    Vec2 operator-(const Vec2& comp){
      return {x-comp.x, y-comp.y};
    }

  } Vec2, WorldPos, ScreenPos;

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

}
#endif