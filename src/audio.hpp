#ifndef AUDIO_H
#define AUDIO_H

/* SDL_mixer Library */
#include <SDL.h>
#include <SDL_mixer.h>

/* Standard Libaries */
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Audio {
   public:
    Audio(int channels, int chunksize);  // open audio

    // Methods
    void LoadMusic(const char *music_path);
    void PlayMusic();
    static void ChangeVolume(int music_volume);
    void FreeResources();

   private:
    // Variables
    Mix_Music *music = nullptr;
};

#endif  // AUDIO_H
