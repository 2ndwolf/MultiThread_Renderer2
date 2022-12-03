#include <memory>
#include <string>

#include <SDL_ttf.h>

#include "AT.h"
#include "identifier.h"
#include "information.h"
#include "window.h"
#include "renderable.h"

// using namespace FK;



namespace FK{

  void AT::setGroupAsRotationCenter(std::shared_ptr<SpriteGroup> sG){

    Vec2 minOffset = {INT_MAX,INT_MAX};
    Box groupBox = {0,0};

    for(std::shared_ptr<ORE::Renderable> s : sG->sprites){
      SpriteInformation& sInfo = s->getInformation();
      SDL_Rect* sdlRect = s->getSDLRect();

      // Ajouter les calculs de caméra
      groupBox.width  = std::max(groupBox.width,  sdlRect->w + sInfo.offset.x);
      groupBox.height = std::max(groupBox.height, sdlRect->h + sInfo.offset.y);

      minOffset.x = std::min(minOffset.x, sInfo.offset.x);
      minOffset.y = std::min(minOffset.y, sInfo.offset.y);
    }

    Vec2 midSize = (Vec2) {minOffset.x + groupBox.width, minOffset.y + groupBox.height} / (Vec2) {2,2};

    for(std::shared_ptr<ORE::Renderable> s : sG->sprites){
      s->setRotationCenter(midSize - s->getInformation().offset);
    }
  }
}