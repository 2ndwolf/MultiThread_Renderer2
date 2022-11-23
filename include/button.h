#ifndef _SDLA_button_h
#define _SDLA_button_h

#include <functional>
#include <memory>

#include "primitives.h"
#include "rendering.h"
#include "identifier.h"

namespace SDLA{
  class Button{
    public:
    Button(Bounds bounds) {this->bounds = bounds;};
    Bounds bounds;

    std::function<void(int)> action;

    int parameter;
    inline static std::map<ID, std::vector<std::shared_ptr<Button>>> buttons;
  };


}

#endif