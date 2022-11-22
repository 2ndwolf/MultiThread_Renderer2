#include <iostream>
#include <vector>
#include <string>

#include <fstream>
#include <sstream>

#include "input.h"
#include "utility.h"
#include "preferences.h"


namespace Preferences {

  void parseIni(const std::string &fileName){
    bool doubleDot = false;

    Input::loadScancodes();

    std::ifstream infile(fileName);
    std::string line;
    while (std::getline(infile, line)){

        if(Utility::strip(line, ' ').rfind("#",0) != std::string::npos) continue;

        if(line.find("..") != std::string::npos){
          doubleDot = true;
          std::cout << Utility::strip(line, '.') << std::endl;
          continue;
        }

        std::vector<std::string> params = Utility::split(line, ':');

        if(params[0].rfind("IMAGE",0) != std::string::npos){
          std::vector<std::string> info = Utility::split(params[0], ' ');
          uPreferences::uPrefs.images.insert({Utility::strip(info[1], ' '), Utility::strip(params[1], ' ')});
          continue;
        }

        if(params[0].rfind("SOUND",0) != std::string::npos){
          std::vector<std::string> info = Utility::split(params[0], ' ');
          uPreferences::uPrefs.sounds.insert({Utility::strip(info[1], ' '), Utility::strip(params[1], ' ')});
          continue;
        }

        if(params[0].rfind("KEY",0) != std::string::npos){
          std::vector<std::string> keyPrefs = Utility::split(params[1], ' ');
          uPreferences::uPrefs.keys.insert({Utility::split(params[0], ' ')[1], Input::findScancode(keyPrefs)});
          continue;
        }
    }

    if(doubleDot) std::cout << "Note: lines containing '..' are printed instead of being parsed." << std::endl;
  }
}