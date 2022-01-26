#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "rendering.h"
#include "primitives.h"
#include "funtimes.h"

// int SDLA::Rendering::threadCount = -1;
std::mutex SDLA::Rendering::mutex;

int main() {
  SDL_Event event;

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
    printf("error initializing SDL: %s\n", SDL_GetError()); 
    return 1;
  }

  SDLA::Rendering rendering = SDLA::Rendering();
  std::shared_ptr<SDLA::Rendering::Window> win = rendering.newWindow(1, (SDLA::Box) {500,500});


  SDLA::Rendering::loadSurface("assets/head440.png", true);
  SDLA::Rendering::loadSurface("assets/head439.png", true);
  SDLA::Rendering::loadSurface("assets/body0.png", true);
  SDLA::Rendering::loadSurface("assets/hat0.png", true);

  SDLA::Rendering::SpriteInfo* groupInfo = new SDLA::Rendering::SpriteInfo();
  groupInfo->angle = 32;
  groupInfo->pos.worldPos.x = 256;
  groupInfo->pos.worldPos.y = 256;

  // groupInfo->rotCenter = new (SDLA::Vec2) {16,20};
  SDLA::Rendering::SpriteInfo* headInfo = new SDLA::Rendering::SpriteInfo();
  headInfo->fileName = "assets/head440.png";
  headInfo->area.box.width = 32;
  headInfo->area.box.height = 32;
  headInfo->area.pos.x = 0;
  headInfo->area.pos.y = 64;
  headInfo->pos.offset.x = 0;
  headInfo->pos.offset.y = 0;
  SDLA::Rendering::SpriteInfo* bodyInfo = new SDLA::Rendering::SpriteInfo();
  bodyInfo->fileName = "assets/body0.png";
  bodyInfo->area.box.width = 32;
  bodyInfo->area.box.height = 32;
  bodyInfo->area.pos.x = 64;
  bodyInfo->area.pos.y = 0;
  bodyInfo->pos.offset.x = 0;
  bodyInfo->pos.offset.y = 16;


  SDLA::Rendering::SpriteInfo* hMenuBackground = new SDLA::Rendering::SpriteInfo();
  hMenuBackground->fileName = "assets/menubg.png";
  hMenuBackground->area.box.width = 16;
  hMenuBackground->area.box.height = 16;
  hMenuBackground->pos.worldPos.x = 10;
  hMenuBackground->pos.worldPos.y = 10;
  SDLA::Box hMenuSize = (SDLA::Box) {30,9};

  SDLA::Rendering::SpriteInfo* bMenuBackground = new SDLA::Rendering::SpriteInfo();
  bMenuBackground->fileName = "assets/menubg.png";
  bMenuBackground->area.box.width = 16;
  bMenuBackground->area.box.height = 16;
  bMenuBackground->pos.worldPos.x = 500 - (10+7*16);
  bMenuBackground->pos.worldPos.y = 10 + 10 * 16;
  SDLA::Box bMenuSize = (SDLA::Box) {7,11};

  SDLA::Rendering::SpriteInfo* sMenuBackground = new SDLA::Rendering::SpriteInfo();
  sMenuBackground->fileName = "assets/menubg.png";
  sMenuBackground->area.box.width = 16;
  sMenuBackground->area.box.height = 16;
  sMenuBackground->pos.worldPos.x = 10;
  sMenuBackground->pos.worldPos.y = 10 + 10 * 16;
  SDLA::Box sMenuSize = (SDLA::Box) {7,11};


  SDLA::idInf headGroup = win->newGroup(0);
  SDLA::Rendering::SpriteInfo* headGrInfo = new SDLA::Rendering::SpriteInfo();
  headGrInfo->pos.worldPos.x = 32;
  headGrInfo->pos.worldPos.y = 26;
  win->updGroup(headGroup.layer, headGrInfo, headGroup.id);
  int spacing = 8;
  int i = 0;
  SDLA::Box headGridSize = {11,3};
  for(int y = 0; y < headGridSize.width; y++){
    for(int x = 0; x < headGridSize.height; x++){
      SDLA::Rendering::SpriteInfo* head = new SDLA::Rendering::SpriteInfo();
      head->fileName = "assets/head" + std::to_string(i) + ".png";
      head->pos.offset = {y * (32 + spacing), x * (32 + spacing)};
      head->area.box.width = 32;
      head->area.box.height = 32;
      head->area.pos.x = 0;
      head->area.pos.y = 64;
      win->updImage(headGroup.layer, head, headGroup.id);
      // headGrid.push_back(head);
      i++;
    }
  }

  // SDLA::Box bodyGridSize = {2,4};
  // std::vector<SDLA::Rendering::SpriteInfo*> bodyGrid;
  // bodyGrid.push_back(new SDLA::Rendering::SpriteInfo());
  // bodyGrid[0]->pos.worldPos.x = 500 - (10+8*16);
  // bodyGrid[0]->pos.worldPos.y = 10 + 11 * 16;
  // // int spacing = 8;
  // i = 0;
  // for(int y = 0; y < bodyGridSize.width; y++){
  //   for(int x = 0; x < bodyGridSize.height; x++){
  //     SDLA::Rendering::SpriteInfo* body = new SDLA::Rendering::SpriteInfo();
  //     body->fileName = "assets/body" + std::to_string(i) + ".png";
  //     body->pos.offset = {y * (32 + spacing), x * (32 + spacing)};
  //     body->area.box.width = 32;
  //     body->area.box.height = 32;
  //     body->area.pos.x = 64;
  //     body->area.pos.y = 0;
  //     bodyGrid.push_back(body);
  //     i++;
  //   }
  // }

  // SDLA::idInf boxId = win->newGroup(0);
  // std::shared_ptr<SDLA::Rendering::SpriteGroup> fds = createBG9(hMenuSize, hMenuBackground);
  // createBG9(bMenuSize, bMenuBackground);
  // createBG9(sMenuSize, sMenuBackground);

  int ms = SDL_GetTicks();
  // int i = 0;

  while(true){
    // i++;
    SDL_PollEvent(&event);
    if(event.type == SDL_QUIT)
      break;
    SDL_Delay(1000 / 500); 
  };

  SDL_Quit();

  return 0;
}