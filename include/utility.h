#ifndef _SDLA_utility_h
#define _SDLA_utility_h

#include <vector>
#include <string>
// #include "utility.h"

namespace FK{
  namespace AT{
    std::string strip(const std::string text, char strip){
      int index = 0, end = text.length();
      while(text[index] == strip) index += 1;
      while(text[end] == strip) end -= 1;
      return text.substr(index, end);
    };

    std::vector<std::string> split(const std::string &text, char sep) {
      std::vector<std::string> tokens;
      size_t start = 0, end = 0;
      while ((end = text.find(sep, start)) != std::string::npos) {
        if (start != end) {
          while (text[start] == sep) start += 1;
          tokens.push_back(strip(text.substr(start, end - start), sep));
        }
        start = end + 1;
      }
      if (end != start) {
        tokens.push_back(strip(text.substr(start, end - start), sep));
      }
      return tokens;
    };
  }
}

#endif