/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "ItemInfoComponent.h"

namespace mmt_gd
{
    ItemInfoComponent::ItemInfoComponent(GameObject& gameObject, int m_value, int m_weight, sf::Vector2f textPos) :
    IComponent(gameObject),
    m_value(m_value),
    m_weight(m_weight),
    m_textPosition(textPos)
    {
        m_originalWindowSize = Game::getWindow().getSize().x;
        m_infoText = tgui::Label::create();
        m_infoText->setText("Pick up " + m_gameObject.getId());
        m_infoText->setVisible(false);
        m_infoText->setOrigin(0.5, 0.5);
        m_infoText->setTextSize(m_fontSize * (m_originalWindowSize / m_madeWindowSize));
        Game::getGui().add(m_infoText);
    }

    void ItemInfoComponent::update(float deltaTime)
    {
        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_infoText->setTextSize(m_infoText->getTextSize() * m_resizeFactor);
        }
        sf::Vector2f worldPos = m_textPosition;
        sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
        m_infoText->setPosition(screenPos.x, screenPos.y);
    }
}
