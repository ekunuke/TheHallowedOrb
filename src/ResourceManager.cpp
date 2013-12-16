#include "ResourceManager.h"

const string ResourceManager::gfxPath = "gfx/";
const string ResourceManager::sfxPath = "sfx/";
const string ResourceManager::musicPath = "sfx/";
const string ResourceManager::fontPath = "gfx/font/";

map<string, sf::Texture*> ResourceManager::textures;
map<string, sf::Font*> ResourceManager::fonts;
map<string, sf::SoundBuffer*> ResourceManager::soundBuffers;
map<string, sf::Music*> ResourceManager::music;
list<sf::Sound*> ResourceManager::playingSounds;

vector<string> ResourceManager::songNames;

sf::Music* ResourceManager::currSong = NULL;
bool ResourceManager::muted = false;
float ResourceManager::musicVolume = 50;
float ResourceManager::sfxVolume = 50;

void ResourceManager::initialize()
{
    loadAllTextures();
    loadAllSounds();
}

void ResourceManager::loadAllTextures()
{
    loadTexture("player_stand.png");
    loadTexture("player_jumpfall.png");
    loadTexture("player_walk.png");
    loadTexture("player_dead.png");
    loadTexture("ugly.png");
    loadTexture("lunger.png");
    loadTexture("lunger_attack.png");
    loadTexture("eye.png");
    loadTexture("bomber.png");
    loadTexture("bouncy.png");
    loadTexture("projectile.png");
    loadTexture("pedestal.png");
    loadTexture("heart.png");
    loadTexture("powerup_health.png");
    loadTexture("boss.png");
    loadTexture("title.png");
    loadTexture("mute.png");
    loadTexture("keyboard.png");
    loadTexture("gamepad.png");
    loadTexture("down.png");
    loadTexture("left.png");
    loadTexture("right.png");
    loadTexture("icon.png");
    loadTexture("throw.png");
    loadTexture("jump.png");

    loadFont("Tiny_04b.ttf");
}

void ResourceManager::loadAllSounds()
{
    loadSound("player_hurt.wav");
    loadSound("player_death.wav");
    loadSound("player_shoot.wav");
    loadSound("projectile_hit.wav");
    loadSound("explode.wav");
    loadSound("big_explosion.wav");
    loadSound("bouncy.wav");
    loadSound("bounce.wav");
    loadSound("powerup.wav");
    loadSound("charge.wav");
    loadSound("landing.wav");
    loadSound("level_change.wav");
    loadSound("boss_horn.wav");
    loadSound("boss_spawned.wav");
    loadSound("boss_summon.wav");

    loadMusic("bgm.ogg");
    loadMusic("ending.ogg");
    loadMusic("boss.ogg");
    loadMusic("static.ogg");
}

void ResourceManager::cleanUp()
{
    map<string, sf::Texture*>::iterator tex_it;
    for(tex_it = textures.begin(); tex_it != textures.end(); tex_it++)
    {
        delete tex_it->second;
        tex_it->second = NULL;
    }
    textures.clear();

    map<string, sf::Font*>::iterator font_it;
     for(font_it = fonts.begin(); font_it != fonts.end(); font_it++)
    {
        delete font_it->second;
        font_it->second = NULL;
    }
    fonts.clear();

    for(list<sf::Sound*>::iterator snd_it = playingSounds.begin(); snd_it != playingSounds.end(); snd_it++)
    {
        delete *snd_it;
    }
    playingSounds.clear();

    map<string, sf::SoundBuffer*>::iterator sfx_it;
    for(sfx_it = soundBuffers.begin(); sfx_it != soundBuffers.end(); sfx_it++)
    {
        delete sfx_it->second;
        sfx_it->second = NULL;
    }
    soundBuffers.clear();

    map<string, sf::Music*>::iterator mus_it;
    for(mus_it = music.begin(); mus_it != music.end(); mus_it++)
    {
        delete mus_it->second;
        mus_it->second = NULL;
    }
    music.clear();
}

bool ResourceManager::loadTexture(string filename)
{
    if(textures.find(filename) != textures.end())
    {
        printf("%s has already been loaded!\n", filename.c_str());
        return false;
    }
    sf::Texture* t = new sf::Texture();
    sf::err().rdbuf(NULL);
    if(!t->loadFromFile(gfxPath + filename))
    {
        if(!t->loadFromFile("../" + gfxPath + filename))
        {
            sf::err().rdbuf(std::cerr.rdbuf());
            printf("Error loading: %s!\n", filename.c_str());
            delete t; t = NULL;
            return false;
        }

    }
            sf::err().rdbuf(std::cerr.rdbuf());
    //printf("Loaded %s!\n", filename.c_str());
    textures.insert(pair<string, sf::Texture*>(filename, t));
    return true;
}

void ResourceManager::unloadTexture(string filename)
{
    map<string, sf::Texture*>::iterator tex_it = textures.find(filename);
    if(tex_it != textures.end())
    {
        delete tex_it->second;
        textures.erase(tex_it);
    }
}

bool ResourceManager::loadFont(string filename)
{
    if(fonts.find(filename) != fonts.end())
    {
        printf("%s has already been loaded!", filename.c_str());
        return false;
    }
    sf::Font* font = new sf::Font();
    sf::err().rdbuf(NULL);
    if(!font->loadFromFile(fontPath + filename))
    {
        if(!font->loadFromFile("../" + fontPath + filename))
        {
            sf::err().rdbuf(std::cerr.rdbuf());
            printf("Error loading: %s!\n", filename.c_str());
            delete font; font = NULL;
            return false;
        }
    }
    sf::err().rdbuf(std::cerr.rdbuf());
    const_cast<sf::Texture&>(font->getTexture(8)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(12)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(16)).setSmooth(false);
    const_cast<sf::Texture&>(font->getTexture(24)).setSmooth(false);
    fonts.insert(pair<string, sf::Font*>(filename, font));
    return true;
}

void ResourceManager::unloadFont(string filename)
{
    map<string, sf::Font*>::iterator font_it = fonts.find(filename);
    if(font_it != fonts.end())
    {
        delete font_it->second;
        fonts.erase(font_it);
    }
}

bool ResourceManager::loadSound(string filename)
{
    if(soundBuffers.find(filename) != soundBuffers.end())
    {
        printf("%s has already been loaded!", filename.c_str());
        return false;
    }
    sf::err().rdbuf(NULL);
    sf::SoundBuffer* soundBuff = new sf::SoundBuffer();
    if(!soundBuff->loadFromFile(sfxPath + filename))
    {
        if(!soundBuff->loadFromFile("../" + sfxPath + filename))
        {
            sf::err().rdbuf(std::cerr.rdbuf());
            printf("Error loading: %s!\n", filename.c_str());
            delete soundBuff;
            soundBuff = NULL;
            return false;
        }
    }
    sf::err().rdbuf(std::cerr.rdbuf());
    soundBuffers.insert(pair<string, sf::SoundBuffer*>(filename, soundBuff));
    return true;
}

void ResourceManager::unloadSound(string filename)
{
    map<string, sf::Font*>::iterator font_it = fonts.find(filename);
    if(font_it != fonts.end())
    {
        delete font_it->second;
        fonts.erase(font_it);
    }
}

bool ResourceManager::loadMusic(string filename)
{
    if(music.find(filename) != music.end())
    {
        printf("%s has already been loaded!", filename.c_str());
        return false;
    }

    sf::Music* m = new sf::Music();
    m->setLoop(true);
    m->setVolume(50);

    sf::err().rdbuf(NULL);
    if(!m->openFromFile(musicPath+filename))
    {
        if(!m->openFromFile("../" + musicPath + filename))
        {
            sf::err().rdbuf(std::cerr.rdbuf());
            printf("Error loading: %s!\n", filename.c_str());
            delete m; m = NULL;
            return false;
        }
    }
    sf::err().rdbuf(std::cerr.rdbuf());
    music.insert(pair<string, sf::Music*>(filename, m));
    return true;
}

void ResourceManager::unloadMusic(string filename)
{
    map<string, sf::Music*>::iterator mus_it = music.find(filename);
    if(mus_it != music.end())
    {
        delete mus_it->second;
        music.erase(mus_it);
    }
}

sf::Texture& ResourceManager::getTexture(string filename)
{
    if(textures.find(filename) == textures.end())
    {
        printf("ERROR: Unable to get texture: %s\n", filename.c_str()); fflush(stdout);
    }
    return *(textures.find(filename)->second);
}

sf::Font& ResourceManager::getDefaultFont()
{
    if(fonts.size() == 0)
        printf("ERROR: No default font found!\n");
    return *(fonts.begin()->second);
}

sf::Font& ResourceManager::getFont(string filename)
{
    return *(fonts.find(filename)->second);
}

sf::SoundBuffer& ResourceManager::getSoundBuffer(string filename)
{
    return *(soundBuffers.find(filename)->second);
}

void ResourceManager::playSound(string filename)
{
//    if(muted)
//        return;
    playingSounds.push_back(new sf::Sound(*(soundBuffers.find(filename)->second)));
    playingSounds.back()->play();
    playingSounds.back()->setVolume(50);

    ///remove sounds
    list<sf::Sound*>::iterator snd_it = playingSounds.begin();
    for(;snd_it != playingSounds.end(); snd_it++)
    {
        if((*snd_it)->getStatus() == sf::Sound::Stopped)
        {
            delete *snd_it;
            playingSounds.erase(snd_it);
            snd_it = playingSounds.begin();
        }
    }
}

sf::Music& ResourceManager::getMusic(string filename)
{
    return *(music.find(filename)->second);
}

sf::Music* ResourceManager::getMusicPointer(string filename)
{
    if(music.find(filename) == music.end())
    {
        printf("Unable to find song: %s!\n", filename.c_str());
        return NULL;
    }
    return music.find(filename)->second;
}

void ResourceManager::playSong(string filename)
{
    sf::Music* prevSong = currSong;
    currSong = getMusicPointer(filename);
    if(muted)
    {
        currSong->setVolume(0);
    }
    currSong->play();
    if(filename.compare("static.ogg") == 0)
    {
        printf("static! setting old prevSong\n");
        currSong = prevSong;
    }
}

void ResourceManager::stopSong(string filename)
{
    sf::Music* prevSong = currSong;
    currSong = getMusicPointer(filename);
    if(currSong)
    {
        currSong->stop();
    }
    currSong = prevSong;
}

void ResourceManager::toggleMute()
{
    muted = !muted;
    if(currSong)
    {
        if(!muted)
        {
            currSong->setVolume(50);
        }
        else
        {
            currSong->setVolume(0);
        }
    }
}

void ResourceManager::setSfxVolume(float volume)
{
    sfxVolume = volume;
}

void ResourceManager::setCurrMusicVolume(float volume)
{
    if(currSong)
    {
         if(muted)
        {
            currSong->setVolume(0);
        }
        else
        {
            currSong->setVolume(volume);
        }
    }
}
void ResourceManager::setMusicVolume(string filename, float volume)
{
    sf::Music* prevSong = currSong;
    currSong = getMusicPointer(filename);
    if(currSong)
    {
         if(muted)
        {
            currSong->setVolume(0);
        }
        else
        {
            currSong->setVolume(volume);
        }
    }
    currSong = prevSong;
}
bool ResourceManager::isMuted()
{
    return muted;
}
