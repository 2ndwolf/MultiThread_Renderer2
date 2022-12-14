#ifndef _MTR_RENDERUPDRNDES_h_
#define _MTR_RENDERUPDRNDES_h_

#include <vector>
#include <map>

#include "layer.h"
#include "image.h"
#include "text.h"
#include "spriteGroup.h"
#include "superGroup.h"
#include "identifier.h"


namespace MTR{

  class RenderUpdates{
    public:
    std::vector<                MTR::RND::Layer       >  updLayer      ;
    std::vector<std::map<void*, MTR::RND::SpriteGroup*>> updSpriteGroup;
                std::map<void*, MTR::RND::SuperGroup* >  updSuperGroup ;

    RenderUpdates(int layers){
      updLayer       = std::vector<                MTR::RND::Layer       > (layers);
      updSpriteGroup = std::vector<std::map<void*, MTR::RND::SpriteGroup*>>(layers);
      updSuperGroup  =             std::map<void*, MTR::RND::SuperGroup *> ();
    };

    RenderUpdates(){};

    ~RenderUpdates(){
      std::map<void*, MTR::RND::SpriteGroup*>::iterator it;
      for(int i = 0; i < updSpriteGroup.size(); i++){
        for(it = updSpriteGroup[i].begin(); it != updSpriteGroup[i].end(); it++){
          delete it->second;
        }
      }
      std::map<void*, MTR::RND::SuperGroup *>::iterator itt;
      for(itt = updSuperGroup.begin(); itt != updSuperGroup.end(); itt++){
        delete itt->second;
      }
    }

    enum updType{
      LAYER,
      SPRITEGROUP,
      SUPERGROUP,
      LENGTH
    };

  };
}


#endif