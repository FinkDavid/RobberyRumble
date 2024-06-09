/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "FinishState.h"
#include "Game.hpp"
#include "GameObjectManager.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "PlayerPickUpComponent.h"

#include "TGUI/TGUI.hpp"

namespace mmt_gd
{
    using namespace std;

    void FinishState::init()
    {
        PROFILE_FUNCTION();

        if (m_isInit)
        {
            return;
        }

        if (!m_font.loadFromFile("../assets/arial.ttf"))
        {
            sf::err() << "Could not load font\n";
            return;
        }

        m_originalWindowSize = Game::getWindow().getSize().x;
        float yWindowSize = m_originalWindowSize * (1080.0f / 1920.0f);

        float factor = m_originalWindowSize / m_madeWindoWsize;

        m_view = m_game->getWindow().getView();
        m_view.setSize(static_cast<float>(m_originalWindowSize), static_cast<float>(yWindowSize));
        m_view.setCenter(static_cast<float>(m_originalWindowSize) / 2.f, static_cast<float>(yWindowSize) / 2.f);

        m_game->getGui().loadWidgetsFromFile("../assets/endScreen.txt");

        if (m_toMainMenuBtn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("ToMainMenuBtn")))
        {
            m_toMainMenuBtn->setTextSize(m_toMainMenuBtn->getTextSize() * factor);
            m_toMainMenuBtn->setSize(m_toMainMenuBtn->getSize().x * factor, m_toMainMenuBtn->getSize().y * factor);
            m_toMainMenuBtn->onClick([&manager = m_gameStateManager] { manager->setState("MenuState"); });
            m_toMainMenuBtn->setOrigin(0.5, 0.5);
            m_toMainMenuBtn->setPosition(m_originalWindowSize / 2.f, (yWindowSize / 2.0f) - (75.f * factor));
        }

        if (m_toGameBtn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("ToGameBtn")))
        {
            m_toGameBtn->setTextSize(m_toGameBtn->getTextSize() * factor);
            m_toGameBtn->setSize(m_toGameBtn->getSize().x * factor, m_toGameBtn->getSize().y * factor);
            m_toGameBtn->onClick([&manager = m_gameStateManager] { manager->setState("MainState"); });
            m_toGameBtn->setOrigin(0.5, 0.5);
            m_toGameBtn->setPosition(m_originalWindowSize / 2.0f, yWindowSize / 2.0f);
        }

        if (m_endGameBtn = dynamic_pointer_cast<tgui::Button>(m_game->getGui().get("ExitGameBtn")))
        {
            m_endGameBtn->setTextSize(m_endGameBtn->getTextSize() * factor);
            m_endGameBtn->setSize(m_endGameBtn->getSize().x * factor, m_endGameBtn->getSize().y * factor);
            m_endGameBtn->onClick([this] { m_game->getWindow().close(); });
            m_endGameBtn->setOrigin(0.5, 0.5);
            m_endGameBtn->setPosition((m_originalWindowSize / 2.f), (yWindowSize / 2.f + (75.f * factor)));
        }

        int player0Score = GameObjectManager::getInstance()
                               .getGameObject("Player0")
                               ->getComponent<PlayerPickUpComponent>()
                               ->getScore();
        int player1Score = GameObjectManager::getInstance()
                               .getGameObject("Player1")
                               ->getComponent<PlayerPickUpComponent>()
                               ->getScore();

        if (m_scoreP0 = dynamic_pointer_cast<tgui::Label>(m_game->getGui().get("Player0Score")))
        {
            m_scoreP0->setPosition(m_scoreP0->getPosition().x * factor, m_scoreP0->getPosition().y * factor);
            m_scoreP0->setTextSize(m_scoreP0->getTextSize() * factor);
            m_scoreP0->setText("Score: " + std::to_string(player0Score));
        }

        if (m_scoreP1 = dynamic_pointer_cast<tgui::Label>(m_game->getGui().get("Player1Score")))
        {
            m_scoreP1->setPosition(m_scoreP1->getPosition().x * factor, m_scoreP1->getPosition().y * factor);
            m_scoreP1->setTextSize(m_scoreP1->getTextSize() * factor);
            m_scoreP1->setText("Score: " + std::to_string(player1Score));
        }

        if (player0Score > player1Score)
        {
            m_finishImage.loadFromFile("../assets/EndScreenRed.png");
        }
        else if (player1Score > player0Score)
        {
            m_finishImage.loadFromFile("../assets/EndScreenBlue.png");
        }
        else
        {
            m_finishImage.loadFromFile("../assets/EndScreenDraw.png");
        }

        m_finishTexture.loadFromImage(m_finishImage);
        m_finishSprite.setTexture(m_finishTexture);
        m_game->getWindow().setView(m_view);
        m_finishSprite.setScale(m_originalWindowSize / m_finishSprite.getLocalBounds().width,
                                yWindowSize / m_finishSprite.getLocalBounds().height);
    }

    void FinishState::update(float delta)
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
            m_scoreP0->setTextSize(m_scoreP0->getTextSize() * m_resizeFactor);
            m_scoreP1->setTextSize(m_scoreP0->getTextSize() * m_resizeFactor);
            m_endGameBtn->setSize(m_endGameBtn->getSize() * m_resizeFactor);
            m_endGameBtn->setTextSize(m_endGameBtn->getTextSize() * m_resizeFactor);
            m_toGameBtn->setSize(m_toGameBtn->getSize() * m_resizeFactor);
            m_toGameBtn->setTextSize(m_toGameBtn->getTextSize() * m_resizeFactor);
            m_toMainMenuBtn->setSize(m_toMainMenuBtn->getSize() * m_resizeFactor);
            m_toMainMenuBtn->setTextSize(m_toMainMenuBtn->getTextSize() * m_resizeFactor);
        }
        
        m_scoreP0->setPosition(m_scoreP0->getPosition().x * m_resizeFactor, m_scoreP0->getPosition().y * m_resizeFactor);
        m_scoreP1->setPosition(m_scoreP1->getPosition().x * m_resizeFactor, m_scoreP1->getPosition().y * m_resizeFactor);
        m_endGameBtn->setPosition(m_endGameBtn->getPosition().x * m_resizeFactor, m_endGameBtn->getPosition().y * m_resizeFactor);
        m_toGameBtn->setPosition(m_toGameBtn->getPosition().x * m_resizeFactor, m_toGameBtn->getPosition().y * m_resizeFactor);
        m_toMainMenuBtn->setPosition(m_toMainMenuBtn->getPosition().x * m_resizeFactor, m_toMainMenuBtn->getPosition().y * m_resizeFactor);
    }

    void FinishState::draw()
    {
        PROFILE_FUNCTION();

        m_game->getWindow().draw(m_text);
        m_game->getWindow().draw(m_finishSprite);
    }

    void FinishState::exit()
    {
        PROFILE_FUNCTION();

        m_game->getGui().removeAllWidgets();
        GameObjectManager::getInstance().shutdown();
    }
}
