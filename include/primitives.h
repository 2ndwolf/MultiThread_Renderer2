#ifndef _SDLA_primitives_h
#define _SDLA_primitives_h

namespace SDLA {

  typedef struct Vec2{
    int x = 0, y = 0;

    bool operator==(const Vec2& comp){
      return x == comp.x && y == comp.y;
    }

  } Vec2, WorldPos, ScreenPos;

  typedef struct Box{
    int width = 0, height = 0;

    bool operator==(const Box& comp){
      return width == comp.width && height == comp.height;
    }
    
  } Box;

  typedef struct {
    Vec2 pos;
    Box box;
  } Bounds;

}
#endif