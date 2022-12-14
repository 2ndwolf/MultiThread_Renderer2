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
      void setSurface (const std::string& newText);
      void setText    (const std::string& newText);

      // Renderable overload
      Text(std::vector<std::string> pwindows):
      Image(pwindows)
      {};

      static void deepCopy(Text* source, Text* target);
      // std::string text;
      SUR::Font font;
      int size;
      Color textColor;

      static SUR::Font loadFont(const std::string& fileName, int size);

    };
  }
}


#endif