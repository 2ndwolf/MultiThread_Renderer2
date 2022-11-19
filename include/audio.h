//=================================
// include guard
#ifndef _audio_h_
#define _audio_h_

#include <vector>
#include <map>
#include <string>

#include "SDL_mixer.h"
#include "SDL_scancode.h"


namespace Audio{

  class Device{
    public:
    static SDL_AudioSpec AudioSpecWant, AudioSpecHave;
    static SDL_AudioDeviceID deviceId;
    static std::vector<SDL_AudioDeviceID> deviceIds;
    static int currentId;
    // static std::map<Uint8*, Uint32> Devices;

  };
  
  class Sound{
    public:
    Uint32 wav_length;
    Uint8 *wav_buffer;
  };

  static const char* MeowMix[32] =
    {"G","Gb","E","D",
    "G","Gb","E","D",
    "G","Gb","G","Ab",
    "A","Gb","E","D",
    "A","Gb","E","D",
    "A","Gb","E","D",
    "A","Gb","A","Bb",
    "B","G","E","D"};
    
  static int currentNote;

  class MeowCtave{
    public:
    static std::map<SDL_Scancode, char*> octave;
  };

  void init();
  void devices();
  void meowctave();


  void play(char * soundName);
  void closeAllAudioDevices();


}

#endif