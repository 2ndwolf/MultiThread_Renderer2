#ifndef _MTR_INSTANTIATORPOWER_h_
#define _MTR_INSTANTIATORPOWER_h_

#include <string>

#include "image.h"
#include "surfaces.h"

namespace MTR{


  RND::Image* createImage(
    std::string fileName, 
    Vec2 offset, 
    int layer,
    std::vector<std::string> windows);

  RND::Text* createText(
    SUR::Font font, 
    std::string text,
    Vec2 offset, 
    int layer,
    std::vector<std::string> windows, 
    Color color);

}



#endif