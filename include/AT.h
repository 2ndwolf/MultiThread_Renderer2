#ifndef _FK_AT_h_
#define _FK_AT_h_

#include <string>
#include <memory>
#include <vector>

#include <SDL.h>

#include "primitives.h"
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
    //     Box windowSize,
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
    std::shared_ptr<Text> loadText(std::string window, int layer, std::shared_ptr<TextInformation> txtInformation, bool ignoreCamera);

  }
}

#endif