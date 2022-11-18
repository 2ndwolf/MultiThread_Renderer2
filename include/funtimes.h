#include <memory>
#include <rendering.h>
  
std::shared_ptr<SDLA::Rendering::SpriteGroup> createBG9(SDLA::Box sizeInBlocks, SDLA::Rendering::SpriteInfo* bgInfo){
  std::vector<SDLA::Rendering::SpriteInfo*> menuBackground;
  menuBackground.push_back(new SDLA::Rendering::SpriteInfo());
  menuBackground[0]->pos.worldPos = bgInfo->pos.worldPos;
  menuBackground[0]->pos.offset = bgInfo->pos.offset;

  SDLA::Box tileSize = (SDLA::Box) {bgInfo->area.box.width, bgInfo->area.box.height};

  for(int y = 0; y < sizeInBlocks.height; y++){
    for(int x = 0; x < sizeInBlocks.width; x++){
      SDLA::Rendering::SpriteInfo* menuElement = new SDLA::Rendering::SpriteInfo();
      menuElement->fileName = bgInfo->fileName;
      menuElement->area.box.width = tileSize.width;
      menuElement->area.box.height = tileSize.height;
      menuElement->pos.offset.y = tileSize.height * y;

      if(y == 0){
        menuElement->area.pos.y = bgInfo->area.pos.y + 0;
      } else if (y == sizeInBlocks.height - 1){
        menuElement->area.pos.y = bgInfo->area.pos.y + tileSize.height * 2;
      } else {
        menuElement->area.pos.y = bgInfo->area.pos.y + tileSize.height;
      }

      menuElement->pos.offset.x = tileSize.width * x;

      if(x == 0){
        menuElement->area.pos.x = bgInfo->area.pos.x + 0;
      } else if (x == sizeInBlocks.width - 1){
        menuElement->area.pos.x = bgInfo->area.pos.x + tileSize.width * 2;
      } else {
        menuElement->area.pos.x = bgInfo->area.pos.x + tileSize.width;
      }

      menuBackground.push_back(menuElement);
    }
  }
  return SDLA::Rendering::Sprite::addImageGroup(0, menuBackground);
}
