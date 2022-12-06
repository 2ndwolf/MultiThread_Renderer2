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

    FK::AT::SpriteInformation::Busy(sG->myWindow);

    for(int i = 0; i < sG->sprites.size(); i++){
      SpriteInformation* sInfo = sG->sprites[i]->sprInfoRead;
      SDL_Rect* tgtRect =  sG->sprites[i]->getSDLRect();

      // Ajouter les calculs de caméra
      groupBox.width  = std::max(groupBox.width,  tgtRect->w + sInfo->bounds.pos.x);
      groupBox.height = std::max(groupBox.height, tgtRect->h + sInfo->bounds.pos.y);

      minOffset.x = std::min(minOffset.x, sInfo->bounds.pos.x);
      minOffset.y = std::min(minOffset.y, sInfo->bounds.pos.y);
      delete sInfo;
    }

    Vec2 midSize = (Vec2) {minOffset.x + groupBox.width, minOffset.y + groupBox.height} / (Vec2) {2,2};

    for(int i = 0; i < sG->sprites.size(); i++){
      std::shared_ptr<FK::AT::SpriteInformation> sInfo = sG->sprites[i]->sprInfoWrite;
       sInfo->rotCenter = new Vec2{0,0};
      *sInfo->rotCenter = midSize -  sInfo->bounds.pos;
      // delete sInfo;
    }
  }
}