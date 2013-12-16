#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "toolbox.h"

class ResourceManager
{
    public:
        static const string gfxPath;
        static const string sfxPath;
        static const string musicPath;
        static const string fontPath;
    private:
        static map<string, sf::Texture*> textures;
        static map<string, sf::Font*> fonts;
        static map<string, sf::SoundBuffer*> soundBuffers;
        static map<string, sf::Music*> music;

        static list<sf::Sound*> playingSounds;

        static sf::Music* currSong;
    public:
        static vector<string> bgNames;
        static vector<string> songNames;
        static vector<string> doodadTextures;
        static vector<string> levelNames;

        static void initialize();
        static void loadAllTextures();
        static void loadAllSounds();
        static void cleanUp();

        static bool loadTexture(string filename);
        static bool loadFont(string filename);
        static bool loadSound(string filename);
        static bool loadMusic(string filename);

        static void unloadTexture(string filename);
        static void unloadFont(string filename);
        static void unloadSound(string filename);
        static void unloadMusic(string filename);

        static sf::Texture& getTexture(string filename);

        static sf::Font& getDefaultFont();
        static sf::Font& getFont(string filename);

        static sf::SoundBuffer& getSoundBuffer(string filename);
        static void playSound(string filename);

        static sf::Music& getMusic(string filename);
        static sf::Music* getMusicPointer(string filename);
        static void playSong(string filename);
        static void stopSong(string filename);

        static void setSfxVolume(float volume);
        static void setCurrMusicVolume(float volume);
        static void setMusicVolume(string filename, float volume);
        static void toggleMute();
        static bool isMuted();
    private:
        static bool muted;
        static float musicVolume;
        static float sfxVolume;
};

#endif // RESOURCEMANAGER_H
