/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "PlayerItemDisplayComponent.h"

namespace mmt_gd
{
    PlayerItemDisplayComponent::PlayerItemDisplayComponent(GameObject& gameObject, int playerIdx) :
    IComponent(gameObject),
    m_playerIndex(playerIdx)
    {
        m_originalWindowSize = Game::getWindow().getSize().x;
        m_icon = tgui::Picture::create();
        m_icon->setSize(m_iconSize * (m_originalWindowSize / m_madeWindowSize), m_iconSize * (m_originalWindowSize / m_madeWindowSize));
        m_icon->setOrigin(0.5, 0.5);
        Game::getGui().add(m_icon);
    }

    void PlayerItemDisplayComponent::update(float deltaTime)
    {
        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_icon->setSize(m_icon->getSize().x * m_resizeFactor, m_icon->getSize().y * m_resizeFactor);
        }

        sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16, m_gameObject.getPosition().y - 30.0f);
        sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
        m_icon->setPosition(screenPos.x, screenPos.y);
    }

    void PlayerItemDisplayComponent::updateIcon(Texture texture)
    {
        m_iconTexture.loadFromPixelData(texture.getSize(), texture.copyToImage().getPixelsPtr());
        m_icon->getRenderer()->setTexture(m_iconTexture);
    }

    void PlayerItemDisplayComponent::showIcon(bool isVisible)
    {
        m_icon->getRenderer()->setOpacity(isVisible);
    }

    void PlayerItemDisplayComponent::setView(sf::View view)
    {
        m_view = view;
    }
}
