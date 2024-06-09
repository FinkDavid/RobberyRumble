/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "PlayerProgressBar.h"

namespace mmt_gd
{
    PlayerProgressBar::PlayerProgressBar(GameObject& gameObject, int playerIdx) :
    IComponent(gameObject),
    m_playerIndex(playerIdx)
    {
        m_originalWindowSize = Game::getWindow().getSize().x;
        m_progressBar = tgui::ProgressBar::create();
        m_progressBar->setVisible(false);
        m_progressBar->setSize(m_progressBarSize.x * (m_originalWindowSize / m_madeWindowSize),
                               m_progressBarSize.y * (m_originalWindowSize / m_madeWindowSize));
        m_progressBar->setOrigin(0.5, 0.5);

        Game::getGui().add(m_progressBar);
    }

    void PlayerProgressBar::update(float deltaTime)
    {
        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_progressBar->setSize(m_progressBar->getSize().x * m_resizeFactor, m_progressBar->getSize().y * m_resizeFactor);
        }
        sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16, m_gameObject.getPosition().y + 10.0f);
        sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
        m_progressBar->setPosition(screenPos.x, screenPos.y);
    }

    void PlayerProgressBar::updateProgressBar(float value)
    {
        value *= 100.0f;
        if (value > m_progressBar->getMaximum())
        {
            m_progressBar->setValue(m_progressBar->getMaximum());
        }
        else if (value < m_progressBar->getMinimum())
        {
            m_progressBar->setValue(m_progressBar->getMinimum());
        }
        else
        {
            m_progressBar->setValue(value);
        }
    }

    void PlayerProgressBar::showProgressBar(bool isVisible)
    {
        m_progressBar->setVisible(isVisible);
    }

    void PlayerProgressBar::setProgressBarColor(tgui::Color color)
    {
        m_progressBar->getRenderer()->setFillColor(color);
    }

    void PlayerProgressBar::setMaxMinValue(float max, float min)
    {
        m_progressBar->setMaximum(max * 100);
        m_progressBar->setMinimum(min * 100);
    }

    void PlayerProgressBar::setView(sf::View view)
    {
        m_view = view;
    }
}
