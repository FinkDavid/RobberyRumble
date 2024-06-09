/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "AssetManager.h"
#include "Game.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "MenuState.hpp"

#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
    using namespace std;

    void MenuState::init()
    {
        PROFILE_FUNCTION();

        if (m_isInit)
        {
            return;
        }

        m_view = m_game->getWindow().getView();

        m_originalWindowSize = Game::getWindow().getSize().x;
        float yWindowSize = m_originalWindowSize * (1080.0f / 1920.0f);

        float factor = m_originalWindowSize / m_madeWindoWsize;

        m_view = m_game->getWindow().getView();
        m_view.setSize(static_cast<float>(m_originalWindowSize), static_cast<float>(yWindowSize));
        m_view.setCenter(static_cast<float>(m_originalWindowSize) / 2.f, static_cast<float>(yWindowSize) / 2.f);

        m_backgroundImage.loadFromFile("../assets/StartScreen.png");
        m_backgroundTexture.loadFromImage(m_backgroundImage);
        m_backgroundSprite.setTexture(m_backgroundTexture);
        m_game->getWindow().setView(m_view);
        m_backgroundSprite.setScale(m_originalWindowSize / m_backgroundSprite.getLocalBounds().width,
                                    yWindowSize / m_backgroundSprite.getLocalBounds().height);

        m_game->getGui().loadWidgetsFromFile("../assets/mainMenu.txt");

        if (m_startButton = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("StartGameBtn")))
        {
            m_startButton->onClick([&manager = m_gameStateManager] { manager->setState("MainState");});
            m_startButton->setTextSize(m_startButton->getTextSize() * factor);
            m_startButton->setSize(m_startButton->getSize().x * factor, m_startButton->getSize().y * factor);
            m_startButton->setOrigin(0.5, 0.5);
            m_startButton->setPosition(m_originalWindowSize / 2.f, (yWindowSize / 2.0f) - (50.0f * factor));
        }

        if (m_exitButton = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("EndGameBtn")))
        {
            m_exitButton->onClick([this] { m_game->getWindow().close(); });
            m_exitButton->setTextSize(m_exitButton->getTextSize() * factor);
            m_exitButton->setSize(m_exitButton->getSize().x * factor, m_exitButton->getSize().y * factor);
            m_exitButton->setOrigin(0.5, 0.5);
            m_exitButton->setPosition(m_originalWindowSize / 2.f, (yWindowSize / 2.0f) + (10 * factor));
        }

        if (m_creditsText = dynamic_pointer_cast<tgui::Label>(m_game->getGui().get("Credits")))
        {
            m_creditsText->setTextSize(m_creditsText->getTextSize() * factor);
            m_creditsText->setPosition(m_originalWindowSize - (275.0f * factor), yWindowSize - (20.0f * factor));
        }
    }

    void MenuState::update(float delta)
    {
        PROFILE_FUNCTION();

        m_game->getWindow().setView(m_view);

        if (InputManager::getInstance().isKeyPressed("Select"))
        {
            m_gameStateManager->setState("MainState");
        }

        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_startButton->setSize(m_startButton->getSize() * m_resizeFactor);
            m_startButton->setTextSize(m_startButton->getTextSize() * m_resizeFactor);
            m_exitButton->setSize(m_exitButton->getSize() * m_resizeFactor);
            m_exitButton->setTextSize(m_exitButton->getTextSize() * m_resizeFactor);
            m_creditsText->setTextSize(m_creditsText->getTextSize() * m_resizeFactor);
        }

        m_startButton->setPosition(m_startButton->getPosition().x * m_resizeFactor, m_startButton->getPosition().y * m_resizeFactor);
        m_exitButton->setPosition(m_exitButton->getPosition().x * m_resizeFactor, m_exitButton->getPosition().y * m_resizeFactor);
        m_creditsText->setPosition(m_creditsText->getPosition().x * m_resizeFactor, m_creditsText->getPosition().y * m_resizeFactor);
    }

    void MenuState::draw()
    {
        PROFILE_FUNCTION();

        m_game->getWindow().draw(m_text);
        m_game->getWindow().draw(m_backgroundSprite);
    }

    void MenuState::exit()
    {
        PROFILE_FUNCTION();

        m_game->getGui().removeAllWidgets();
        GameState::exit();
    }
}
