/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include <TGUI/TGUI.hpp>

using namespace std;
using namespace sf;

namespace mmt_gd
{
    class AssetManager
    {
    public:
        static AssetManager& getInstance();

        void init();

        bool loadTexture(string name, string path);
        Texture& getTexture(string name);

        bool loadFont(string name, string path);
        Font& getFont(string name);

        bool loadSound(string name, string path);
        void AssetManager::playSound(const std::string& soundID);

        bool loadMusic(string name, string path);
        Music& getMusic(string name);

    private:
        AssetManager(void) = default;
        ~AssetManager(void) = default;
        AssetManager(const AssetManager&) = delete;
        AssetManager& operator=(const AssetManager&) = delete;

        map<string, Texture> m_textures;
        map<string, shared_ptr<SoundBuffer>> m_soundBuffers;
        map<string, unique_ptr<Music>> m_music;
        map<string, Font> m_fonts;

        Sound m_sound;
    };
}
