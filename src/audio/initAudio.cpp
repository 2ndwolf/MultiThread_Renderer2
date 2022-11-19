#include <string>
#include <iostream>

#include "SDL_audio.h"
#include "SDL_stdinc.h"

#include "audio.h"
#include "SDL_scancode.h"


using namespace std;

SDL_AudioSpec Audio::Device::AudioSpecWant;
std::vector<SDL_AudioDeviceID> Audio::Device::deviceIds;
int Audio::Device::currentId;
SDL_AudioDeviceID Audio::Device::deviceId;

std::map<SDL_Scancode, char*> Audio::MeowCtave::octave;


namespace Audio{

  void init(){
    devices();
    meowctave();

    currentNote = -1;

  }

  void devices(){
    SDL_memset(&Device::AudioSpecWant, 0, sizeof(Device::AudioSpecWant));
    Device::AudioSpecWant.freq = 44100;
    Device::AudioSpecWant.format = AUDIO_S16;
    Device::AudioSpecWant.channels = 2;
    Device::AudioSpecWant.samples = 4096;

    Device::currentId = -1;
  }

  // void meowctave(){
  //   MeowCtave::octave[SDL_SCANCODE_A] = {"7"};
  //   MeowCtave::octave[SDL_SCANCODE_B] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_C] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_D] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_E] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_F] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_G] = {"3"};
  //   MeowCtave::octave[SDL_SCANCODE_H] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_I] = {"7"};
  //   MeowCtave::octave[SDL_SCANCODE_J] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_K] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_L] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_M] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_N] = {"8"};
  //   MeowCtave::octave[SDL_SCANCODE_O] = {"7"};
  //   MeowCtave::octave[SDL_SCANCODE_P] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_Q] = {"5"};
  //   MeowCtave::octave[SDL_SCANCODE_R] = {"4"};
  //   MeowCtave::octave[SDL_SCANCODE_S] = {"6"};
  // }
}