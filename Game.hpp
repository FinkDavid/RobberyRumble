/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "FPS.hpp"
#include "GameStateManager.hpp"

#include <TGUI/Backend/SFML-Graphics.hpp>
#include <string>

namespace mmt_gd
{
    class InputManager;
    class DebugDraw;
    class AssetManager;

    class Game
    {
    public:
        struct Config
        {
            sf::Vector2i m_resolution{960, 540};
            std::string m_windowName = "RobberyRumble";
        };

        Config& getConfig()
        {
            return m_config;
        }

        static sf::RenderWindow& getWindow();
        
        static tgui::Gui& getGui();

        void run();

    private:
        bool init();
        void update();
        void draw();
        void shutdown() const;

        Config m_config;
        float m_aspectRatio = 960.0f / 540.0f;

        static sf::RenderWindow m_window;
        GameStateManager m_gameStateManager;

        sf::Image m_icon;

        InputManager* m_inputManager = nullptr;
        AssetManager* m_assetManager = nullptr;
        DebugDraw* m_debugDraw = nullptr;
        Fps m_fps;
        static tgui::Gui m_gui;
    };
}
