#ifndef _MTR_TEXT_h_
#define _MTR_TEXT_h_

#include <string>

#include <SDL.h>

#include "image.h"
#include "window.h"
#include "primitives.h"


namespace MTR{

  namespace RND{

    class Text : public Image {
      public:
      // Image overload
      void Text::setSurface    (std::string newText);
      inline void Text::setText(std::string newText){setSurface(newText);};

      // Renderable overload
      static void update(Text* image);


      // std::string text;
      SUR::Font font;
      int size;
      Color textColor;

      static SUR::Font Text::loadFont(std::string fileName, int size);
      // private:
    };
  }
}


#endif