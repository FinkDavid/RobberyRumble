/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "Game.hpp"

#include "AssetManager.h"
#include "DebugDraw.hpp"
#include "InputManager.hpp"
#include "MainState.hpp"
#include "MenuState.hpp"
#include "SFML/Window/Joystick.hpp"

#include <sstream>
#include "FinishState.h"

namespace mmt_gd
{
    using namespace std;
    tgui::Gui Game::m_gui;
    sf::RenderWindow Game::m_window;

    tgui::Gui& Game::getGui()
    {
        return m_gui;
    }

    sf::RenderWindow& Game::getWindow()
    {
        return m_window;
    }

    void Game::run()
    {
        Instrumentor::instance().beginSession("startup", "startup.json");

        if (!init())
        {
            return;
        }
        Instrumentor::instance().endSession();
        Instrumentor::instance().beginSession("runtime", "runtime.json");

        while (m_window.isOpen())
        {
            PROFILE_SCOPE("Frame");
            {
                PROFILE_SCOPE("Process Events");

                sf::Event event{};
                while (m_window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        shutdown();
                        m_window.close();
                        return;
                    }

                    if (event.type == sf::Event::Resized)
                    {
                        float width = event.size.height * m_aspectRatio;
                        m_window.setSize(sf::Vector2u(width, event.size.height));
                    }

                    m_inputManager->process(event);
                    m_gui.handleEvent(event);
                }
            }
            update();
            draw();
        }

        shutdown();
    }

    bool Game::init()
    {
        PROFILE_FUNCTION();

        m_inputManager = &InputManager::getInstance();
        m_assetManager = &AssetManager::getInstance();
        m_debugDraw = &DebugDraw::getInstance();

        m_inputManager->init();
        m_assetManager->init();

        m_gameStateManager.registerState("MenuState", make_shared<MenuState>(&m_gameStateManager, this));
        m_gameStateManager.registerState("MainState", make_shared<MainState>(&m_gameStateManager, this));
        m_gameStateManager.registerState("FinishState", make_shared<FinishState>(&m_gameStateManager, this));
        m_gameStateManager.setState("MenuState");

        m_window.create(sf::VideoMode(m_config.m_resolution.x, m_config.m_resolution.y), m_config.m_windowName);
        m_window.setFramerateLimit(144);
        m_gui.setTarget(m_window);
        m_gui.setFont("../assets/editundo.ttf");
        m_inputManager->setRenderWindow(&m_window);

        //Taskbar icon
        m_icon = sf::Image{};
        m_icon.loadFromFile("../assets/Icon256x256.bmp");
        m_window.setIcon(m_icon.getSize().x, m_icon.getSize().y, m_icon.getPixelsPtr());
        
        return true;
    }

    void Game::update()
    {
        PROFILE_FUNCTION();

        static sf::Clock clock;
        const auto deltaTime        = clock.restart();
        const auto deltaTimeSeconds = deltaTime.asSeconds();

        m_inputManager->update();
        m_gameStateManager.update(deltaTimeSeconds);
        m_debugDraw->update(deltaTimeSeconds);

        std::ostringstream ss;
        m_fps.update();
        ss << m_config.m_windowName << " | FPS: " << m_fps.getFps();

        m_window.setTitle(ss.str());

        auto music = &m_assetManager->getMusic("BackgroundMusic");
        music->setLoop(true);
        if (music->getStatus() != sf::Music::Status::Playing)
        {
            music->setVolume(30);
            music->play();
        }
    }

    void Game::draw()
    {
        PROFILE_FUNCTION();

        m_window.clear();

        m_gameStateManager.draw();

        m_debugDraw->draw(m_window);

        m_gui.draw();

        m_window.display();
    }

    void Game::shutdown() const
    {
        m_gameStateManager.shutdown();
        GameObjectManager::getInstance().shutdown();
    }
}
