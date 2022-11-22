#include <string>
#include <iostream>

#include "SDL_timer.h"
#include "SDL_audio.h"
#include "SDL_scancode.h"

#include "SDL_mixer.h"

#include "audio.h"


namespace Audio{

  void play(std::string soundName){
    if(LoadedSounds.find((std::string)soundName) == LoadedSounds.end()){
      std::cout << "Loading new sound..." << std::endl;
      Sound sample = Sound();
      if (SDL_LoadWAV(soundName.c_str(), &Device::AudioSpecWant, &sample.wav_buffer, &sample.wav_length) == NULL) {
        fprintf(stderr, "Could not open %s : %s\n", soundName, SDL_GetError());
        return;
      } else {
        LoadedSounds[soundName] = sample;
      }
    }

    if(Device::currentId == -1){
      Device::deviceId = SDL_OpenAudioDevice(NULL, 0, &Device::AudioSpecWant, NULL, 0);
    }

    if(Device::deviceId != 0 && Device::currentId == -1){
      Device::deviceIds.push_back(Device::deviceId);
    } else {
      Device::currentId++;
      Device::deviceId = Device::deviceIds[Device::currentId%Device::deviceIds.size()];
    }

    if(SDL_QueueAudio(Device::deviceId, LoadedSounds[soundName].wav_buffer, LoadedSounds[soundName].wav_length) == 0){
      if(Device::currentId == -1){
        SDL_PauseAudioDevice(Device::deviceId, 0);
      }
    }
    // SDL_Delay(500);
    // SDL_CloseAudioDevice(deviceId);

    // SDL_FreeWAV(LoadedSounds[soundName].wav_buffer);
  }

  void closeAllAudioDevices(){
    Device::currentId = -1;
    for(int i = 0; i < Device::deviceIds.size(); i++){
      SDL_CloseAudioDevice(Device::deviceIds[i]);
      std::cout << "Closing devices!!!!!" + i << std::endl;
    }
    Device::deviceIds.resize(0);
  }
}