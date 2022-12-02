#include <iostream>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

// #include "info.h"
// #include "renderable.h"
#include "input.h"
#include "utility.h"
#include "preferences.h"


namespace FK {
  namespace ORE{

    void parseIni(const std::string &fileName){
      bool doubleDot = false;

      FK::ORE::loadScancodes();

      std::ifstream infile(fileName);
      std::string line;
      while (std::getline(infile, line)){

          if(FK::AT::strip(line, ' ').rfind("#",0) != std::string::npos) continue;

          if(line.find("..") != std::string::npos){
            doubleDot = true;
            std::cout << FK::AT::strip(line, '.') << std::endl;
            continue;
          }

          std::vector<std::string> params = FK::AT::split(line, ':');

          if(params[0].rfind("IMAGE",0) != std::string::npos){
            std::vector<std::string> info = FK::AT::split(params[0], ' ');
            FK::ORE::uPreferences::uPrefs.images.insert({FK::AT::strip(info[1], ' '), FK::AT::strip(params[1], ' ')});
            continue;
          }

          if(params[0].rfind("SOUND",0) != std::string::npos){
            std::vector<std::string> info = FK::AT::split(params[0], ' ');
            FK::ORE::uPreferences::uPrefs.sounds.insert({FK::AT::strip(info[1], ' '), FK::AT::strip(params[1], ' ')});
            continue;
          }

          if(params[0].rfind("KEY",0) != std::string::npos){
            std::vector<std::string> keyPrefs = FK::AT::split(params[1], ' ');
            FK::ORE::uPreferences::uPrefs.keys.insert({FK::AT::split(params[0], ' ')[1], FK::ORE::findScancode(keyPrefs)});
            continue;
          }
      }

      if(doubleDot) std::cout << "Note: lines containing '..' are printed instead of being parsed." << std::endl;
    }
  }
}