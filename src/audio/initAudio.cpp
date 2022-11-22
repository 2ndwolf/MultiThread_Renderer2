#include <string>
#include <iostream>

#include "SDL_audio.h"
#include "SDL_stdinc.h"

#include "audio.h"
#include "SDL_scancode.h"


namespace Audio{

  void init(){
    devices();
  }

  void devices(){
    SDL_memset(&Device::AudioSpecWant, 0, sizeof(Device::AudioSpecWant));
    Device::AudioSpecWant.freq = 44100;
    Device::AudioSpecWant.format = AUDIO_S16;
    Device::AudioSpecWant.channels = 2;
    Device::AudioSpecWant.samples = 4096;

    Device::currentId = -1;
  }

}