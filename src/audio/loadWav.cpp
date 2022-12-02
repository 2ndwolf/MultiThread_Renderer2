#include <string>
#include <iostream>

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "SDL_scancode.h"

#include "SDL_mixer.h"

#include "audio.h"


namespace FK{

  namespace AT{
    void play(std::string soundName){
      if(ORE::LoadedSounds.find((std::string)soundName) == ORE::LoadedSounds.end()){
        std::cout << "Loading new sound..." << std::endl;
        ORE::Sound sample = ORE::Sound();
        if (SDL_LoadWAV(soundName.c_str(), &ORE::Device::AudioSpecWant, &sample.wav_buffer, &sample.wav_length) == NULL) {
          fprintf(stderr, "Could not open %s : %s\n", soundName, SDL_GetError());
          return;
        } else {
          ORE::LoadedSounds[soundName] = sample;
        }
      }

      if(ORE::Device::currentId == -1){
        ORE::Device::deviceId = SDL_OpenAudioDevice(NULL, 0, &ORE::Device::AudioSpecWant, NULL, 0);
      }

      if(ORE::Device::deviceId != 0 && ORE::Device::currentId == -1){
        ORE::Device::deviceIds.push_back(ORE::Device::deviceId);
      } else {
        ORE::Device::currentId++;
        ORE::Device::deviceId = ORE::Device::deviceIds[ORE::Device::currentId%ORE::Device::deviceIds.size()];
      }

      if(SDL_QueueAudio(ORE::Device::deviceId, ORE::LoadedSounds[soundName].wav_buffer, ORE::LoadedSounds[soundName].wav_length) == 0){
        if(ORE::Device::currentId == -1){
          SDL_PauseAudioDevice(ORE::Device::deviceId, 0);
        }
      }
      // SDL_Delay(500);
      // SDL_CloseAudioDevice(deviceId);

      // SDL_FreeWAV(ORE::LoadedSounds[soundName].wav_buffer);
    }
  }

  namespace ORE{
    void closeAllAudioDevices(){
      ORE::Device::currentId = -1;
      for(int i = 0; i < ORE::Device::deviceIds.size(); i++){
        SDL_CloseAudioDevice(ORE::Device::deviceIds[i]);
        std::cout << "Closing devices!!!!!" + i << std::endl;
      }
      ORE::Device::deviceIds.resize(0);
    }
  }
}