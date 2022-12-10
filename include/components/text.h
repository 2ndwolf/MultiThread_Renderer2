#ifndef _MTR_TEXT_h_
#define _MTR_TEXT_h_

#include <string>

#include <SDL.h>

#include "image.h"
// #include "window.h"
#include "primitives.h"


namespace MTR{

  namespace RND{

    class Text : public Image {
      public:
      // Image overload
      void setSurface (std::string newText);
      void setText    (std::string newText);

      // Renderable overload
      static void update(Text* image);

      static void deepCopy(Text* source, Text* target);
      // std::string text;
      SUR::Font font;
      int size;
      Color textColor;

      static SUR::Font loadFont(std::string fileName, int size);
      // private:
    };
  }
}


#endif