#ifndef _MTR_INSTANTIATORPOWER_h_
#define _MTR_INSTANTIATORPOWER_h_

#include <string>

#include "image.h"
#include "surfaces.h"
#include "text.h"

namespace MTR{


  RND::Image* createImage(
    const std::string& fileName, 
    Vec2 offset, 
    int layer,
    std::vector<std::string> windows);

  // This is the one with the right order
  RND::Image* createSprite(const std::string& fileName,
  std::vector<std::string> windows,
  Bounds crop = {0,0,0,0},
  Vec2 offset = {0,0},
  int layer = 0
  );


  RND::Text* createText(
    SUR::Font font, 
    const std::string& text,
    Vec2 offset, 
    int layer,
    std::vector<std::string> windows, 
    Color color);

}



#endif