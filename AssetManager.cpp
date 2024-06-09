/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "AssetManager.h"

namespace mmt_gd
{
    AssetManager& AssetManager::getInstance()
    {
        static AssetManager assetManager;
        return assetManager;
    }

    void AssetManager::init()
    {
        loadFont("EditUndo", "../assets/editundo.ttf");
        loadMusic("BackgroundMusic", "../assets/Sound/BackgroundMusic.ogg");
        loadSound("hit", "../assets/Sound/Hit_damage_1.wav");
        loadSound("deposit", "../assets/Sound/Fruit_collect_1.wav");
        loadSound("collect", "../assets/Sound/Retro6.wav");
        loadSound("drop", "../assets/Sound/Wood Block2.wav");
    }

    bool AssetManager::loadTexture(string name, string path)
    {
        Texture tex;
        if (!tex.loadFromFile(path))
        {
            err() << "Texture not loaded\n";
            return false;
        }
        m_textures[name] = tex;
        return true;
    }

    Texture& AssetManager::getTexture(string name)
    {
        return m_textures.at(name);
    }

    bool AssetManager::loadFont(string name, string path)
    {
        Font font;
        if (!font.loadFromFile(path))
        {
            err() << "Font not loaded\n";
            return false;
        }
        m_fonts[name] = font;
        return true;
    }

    Font& AssetManager::getFont(string name)
    {
        return m_fonts.at(name);
    }

    bool AssetManager::loadSound(string name, string path)
    {
        shared_ptr<SoundBuffer> buffer = make_shared<SoundBuffer>();
        if (!buffer->loadFromFile(path))
        {
            err() << "Sound not loaded\n";
            return false;
        }
        m_soundBuffers[name] = buffer;
        return true;
    }

    void AssetManager::playSound(const std::string& soundID)
    {
        m_sound.setBuffer(*m_soundBuffers.at(soundID));
        if (m_sound.getStatus() != sf::Sound::Status::Playing)
        {
            m_sound.play();
        }
    }

    bool AssetManager::loadMusic(string name, string path)
    {
        unique_ptr<Music> music = make_unique<Music>();
        if (!music->openFromFile(path))
        {
            err() << "Music not loaded\n";
            return false;
        }
        m_music[name] = move(music);
        return true;
    }

    Music& AssetManager::getMusic(string name)
    {
        return *(m_music.at(name));
    }
}
