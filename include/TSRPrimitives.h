#ifndef _SDLA_primitives_h
#define _SDLA_primitives_h

#include <SDL.h>

#include <algorithm>
namespace FK {

  typedef int ID;

}

  struct Vec2 {
    int x = 0, y = 0;

    bool operator==(const Vec2& comp){
      return x == comp.x && y == comp.y;
    }

    Vec2 operator-(const Vec2& comp){
      return {x-comp.x, y-comp.y};
    }

    Vec2 operator+(const Vec2& comp){
      return {x+comp.x, y+comp.y};
    }

    Vec2 operator/(const Vec2 comp){
      return {x/comp.x, y/comp.y};
    }

    Vec2 operator<<(const Vec2 comp){
      return {std::min(x,comp.x), std::min(y,comp.y)};
    }

  };

  typedef struct Box {
    int width, height;

    Box operator+(const Box& comp){
      return {width+comp.width, height+comp.height};
    }

    // Box operator+(const Vec2& comp){
    //   return {width+comp.x, height+comp.y};
    // }

    Box operator-(const Box& comp){
      return {width-comp.width, height-comp.height};
    }

    // BoxPTR operator&(Box& comp){
    //   return {&comp.width, &comp.height};
    // }

    // Base operators
    bool operator==(const Box& comp){
      return width == comp.width && height == comp.height;
    };

    bool operator<(const Box& comp){
      return width < comp.width && height < comp.height;
    }

    bool operator>(const Box& comp){
      return width > comp.width && height > comp.height;
    }

    bool operator>=(const Box& comp){
      return width >= comp.width && height >= comp.height;
    }

  } Box;


  struct Bounds{
    Vec2 pos;
    Box box;

    // Check if two bounds intersect
    // bool operator>=(const Bounds& comp){
    //   Box srcBx = box+comp.box;
    //   Box combinedBx = (*this & comp);

    //   return ((srcBx.width > combinedBx.width) || (srcBx.height > combinedBx.height));
    // };

    // Combine two Bounds
    Bounds operator+(const Bounds& comp){
      return {(pos << pos), (*this & comp)};
    }

    // Combine two Bounds' Boxes
    Box operator&(const Bounds& comp){
       Vec2 tmp = (*this >> comp) - (pos << comp.pos);
       return  {tmp.x,tmp.y};
    };

    // Bottom-Right most point
    Vec2 operator>>(const Bounds& comp){
      return {
          pos.x < comp.pos.x ?
          comp.pos.x - pos.x + comp.box.width :
          pos.x - comp.pos.x + box.width,
          pos.y < comp.pos.y ?
          comp.pos.y - pos.y + comp.box.height :
          pos.y - comp.pos.y + box.height
      };
    };

    bool operator==(const Bounds& comp){
      return box == comp.box & pos == comp.pos;
    };

  };

// }
#endif