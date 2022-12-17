#ifndef _MTR_TEXT_h_
#define _MTR_TEXT_h_

#include <string>
#include <utility>

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

      Text(const Text& old_obj) : Image(old_obj){
        font      = old_obj.font     ;
        size      = old_obj.size     ;
        textColor = old_obj.textColor;
      }

      Text(Text&& other) : Image(other){
        font      = other.font     ;
        size      = other.size     ;
        textColor = other.textColor;

        other.font      = {"",nullptr,0};
        other.size      = 0;
        other.textColor = {0,0,0,0};
      }

      Text& operator=(const Text& other){
        if(this!=&other){
          Text(other).swap(*this);
        }
        return *this;
      }

      void swap(Text& t){
        std::swap(font     , t.font     );
        std::swap(size     , t.size     );
        std::swap(textColor, t.textColor);
      }

      Text& operator=(Text&& other){
        if(this != &other){
          Image::operator=(other);
          font      = other.font     ;
          size      = other.size     ;
          textColor = other.textColor;

          other.font      = {"",nullptr,0};
          other.size      = 0;
          other.textColor = {0,0,0,0};
        }

        return *this;
      }
      
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