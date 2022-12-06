#ifndef _FK_AT_h_
#define _FK_AT_h_

#include <string>
#include <memory>
#include <vector>

#include <SDL.h>

#include "TSRPrimitives.h"
#include "fkore.h"
#include "information.h"
#include "renderables.h"

namespace FK{
  namespace AT{
    bool keyPressed(std::string key);
    bool keyHeld(std::string key);
    bool keyUp(std::string key);
    bool keyDown(std::string key);

    // static std::string newWindow(
    //     int layerCount,
    //     BoxPTR windowSize,
    //     std::string name = "Abstraction",
    //     Vec2 position = {SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED},
    //     SDL_WindowFlags mode = SDL_WINDOW_RESIZABLE,
    //     bool hasOwnThread = false
    // );
    void setGroupAsRotationCenter(std::shared_ptr<FK::AT::SpriteGroup> sG);
    void setGroupRotationCenter(std::shared_ptr<FK::AT::SpriteGroup> sG, Vec2 center);

    std::shared_ptr<SpriteGroup> addSprite(std::string window, int layer, std::shared_ptr<SpriteInformation> info, bool ignoreCamera);
    // worldPos is ignored for grouped sprites (only the group as a whole has a worldPos)
    // static std::vector<std::shared_ptr<Sprite>> addSpriteGroup(std::string window, int layer, std::shared_ptr<Information> groupInformation, std::vector<SpriteInformation*> group);
    std::shared_ptr<SpriteGroup> addSpriteGroup(std::string window, int layer, std::shared_ptr<Information> groupInformation, std::vector<std::shared_ptr<SpriteInformation>> group);
    std::shared_ptr<SpriteGroup> loadText(std::string window, int layer, std::shared_ptr<TextInformation> txtInformation, bool ignoreCamera);

    // void scale(std::shared_ptr<SpriteGroup> sG, Vec2 groupSize = {0,0}){

        // s->sprite_rect.w += s->scale_direction;
        // s->sprite_rect.h += s->scale_direction;

        // if (s->scale_direction > 0) {
        //     if (s->sprite_rect.w >= viewport.w || s->sprite_rect.h >= viewport.h) {
        //         s->scale_direction = -1;
        //     }
        // } else {
        //     if (s->sprite_rect.w <= 1 || s->sprite_rect.h <= 1) {
        //         s->scale_direction = 1;
        //     }
        // }

        // s->sprite_rect.x = (viewport.w - s->sprite_rect.w) / 2;
        // s->sprite_rect.y = (viewport.h - s->sprite_rect.h) / 2;

    // };

  }
}

#endif