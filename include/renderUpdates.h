#ifndef _MTR_RENDERUPDRNDES_h_
#define _MTR_RENDERUPDRNDES_h_

#include <vector>
#include <map>

#include "layer.h"
#include "image.h"
#include "text.h"
#include "spriteGroup.h"
#include "superGroup.h"


namespace MTR{

  class UpdateType{
    // std::vector<std::map<void*, MTR::    Layer      >>::iterator it;
    // std::vector<std::map<void*, MTR::    Layer      >> upd;
    };

  class LayerUpdate   : public UpdateType{public:
  // std::vector<MTR::Layer>>::iterator it;
  std::vector<MTR::RND::Layer> upd;};

  class SpriteUpdate  : public UpdateType{public:
  std::vector<std::map<void*, MTR::RND::Image >>::iterator it;
  std::vector<std::map<void*, MTR::RND::Image >> upd;};

  class TextUpdate   : public UpdateType{public:
  std::vector<std::map<void*, MTR::RND::Text >>::iterator it;
  std::vector<std::map<void*, MTR::RND::Text >> upd;};

  class SpriteGroupUpdate : public UpdateType{public:
  std::vector<std::map<void*, MTR::RND::SpriteGroup>>::iterator it;
  std::vector<std::map<void*, MTR::RND::SpriteGroup>> upd;};

  class SuperGroupUpdate  : public UpdateType{public:
  std::map<void*, MTR::RND::SuperGroup >::iterator it;
  std::map<void*, MTR::RND::SuperGroup > upd;};

  class RenderUpdates{
    public:
    LayerUpdate      * updLayer       = new LayerUpdate      ();
    SuperGroupUpdate * updSuperGroup  = new SuperGroupUpdate ();
    SpriteGroupUpdate* updSpriteGroup = new SpriteGroupUpdate();
    SpriteUpdate     * updSprite      = new SpriteUpdate     ();
    TextUpdate       * updText        = new TextUpdate       ();

    RenderUpdates(int layers){
      updLayer      ->upd = std::vector<                MTR::RND::Layer       >(layers);
      updText       ->upd = std::vector<std::map<void*, MTR::RND::Text       >>(layers);
      updSprite     ->upd = std::vector<std::map<void*, MTR::RND::Image      >>(layers);
      updSpriteGroup->upd = std::vector<std::map<void*, MTR::RND::SpriteGroup>>(layers);
      updSuperGroup ->upd =             std::map<void*, MTR::RND::SuperGroup > ();
    };

    RenderUpdates(){};
    ~RenderUpdates(){
      delete updLayer      ;
      delete updSuperGroup ;
      delete updSpriteGroup;
      delete updSprite     ;
      delete updText       ;
    }

    // protected:

    public:
    enum updType{
      LAYER,
      SPRITE,
      TEXT,
      SPRITEGROUP,
      SUPERGROUP,
      LENGTH
    };

    //   inline UpdateType* checkUpdType(updType upd){
    //   switch(upd){
    //     case(LAYER):
    //       return RenderUpdates::updLayer      ;
    //     break;
    //     case(SPRITE):
    //       return RenderUpdates::updSprite     ;
    //     break;
    //     case(TEXT):
    //       return RenderUpdates::updText       ;
    //     break;
    //     case(SPRITEGROUP):
    //       return RenderUpdates::updSpriteGroup;
    //     break;
    //     case(SUPERGROUP):
    //       return RenderUpdates::updSuperGroup ;
    //     break;
        
    //   };
    // };
  };
}


#endif