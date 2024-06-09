/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "PlayerPickUpComponent.h"

namespace mmt_gd
{
    PlayerPickUpComponent::PlayerPickUpComponent(GameObject& gameObject, int playerIdx, std::string vanId) :
        IComponent(gameObject),
        m_playerIndex(playerIdx),
        m_vanId(vanId)
    {
        m_originalWindowSize = Game::getWindow().getSize().x;
        m_infoText = tgui::Label::create();
        m_infoText->setVisible(false);
        m_infoText->setTextSize(m_infoTextSize * (m_originalWindowSize / m_madeWindowSize));
        m_infoText->setOrigin(0.5, 0.5);
        Game::getGui().add(m_infoText);

        m_valueText = tgui::Label::create();
        m_valueText->setVisible(false);
        m_valueText->setTextSize(m_valueTextSize * (m_originalWindowSize / m_madeWindowSize));
        m_valueText->setOrigin(0.5, 0.5);
        Game::getGui().add(m_valueText);
    }

    void PlayerPickUpComponent::update(float deltaTime)
    {
        if (!m_pickUp)
        {
            m_pickUpTimer += deltaTime;
        }

        m_elapsedTime += deltaTime;

        if (m_pickUpTimer > m_pickUpTime)
        {
            if (m_elapsedTime >= 1.0f)
            {
                if (m_score >= 1)
                {
                    ResetValueText();
                    m_valueText->getRenderer()->setTextColor(tgui::Color(255, 0, 0, 255));
                    m_valueText->setText("-1");
                    m_score -= 1;
                }
                m_elapsedTime = 0.0f;
            }
        }

        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_infoText->setTextSize(m_infoText->getTextSize() * m_resizeFactor);
            m_valueText->setTextSize(m_valueText->getTextSize() * m_resizeFactor);
        }

        //Info Text
        if (m_textDisplayTimer >= m_textDisplayTime)
        {
            m_infoText->setVisible(false);
            m_textDisplayTimer = 0.0f;
            m_textAnimationTimer = 0.0f;
        }

        if (m_infoText->isVisible())
        {
            m_textDisplayTimer += deltaTime;
            m_textAnimationTimer += deltaTime / 0.5f;
            m_textOffsetY = m_textStartPosition +
                            easeOutQuad(m_textAnimationTimer) * (m_textEndPosition - m_textStartPosition);
            m_textAnimationTimer = std::min(m_textAnimationTimer, 1.0f);
            sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16,
                                                 m_gameObject.getPosition().y - m_textOffsetY);
            sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
            m_infoText->setPosition(screenPos.x, screenPos.y);
        }
        else
        {
            m_textOffsetY = m_textStartPosition;
            sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16, m_gameObject.getPosition().y);
            sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
            m_infoText->setPosition(screenPos.x, screenPos.y);
        }

        //ItemValueText
        if (m_valueTextDisplayTimer >= m_valueTextDisplayTime)
        {
            m_valueText->setVisible(false);
            m_valueTextDisplayTimer = 0.0f;
            m_valueTextAnimationTimer = 0.0f;
        }

        if (m_valueText->isVisible())
        {
            m_valueTextDisplayTimer += deltaTime;
            m_valueTextAnimationTimer += deltaTime / m_valueTextDisplayTime;
            float easedProgress = easeOutQuad(m_valueTextAnimationTimer);
            float interpolatedX = m_valueTextStartPosition.x + easedProgress * (m_valueTextEndPosition.x - m_valueTextStartPosition.x);
            float interpolatedY = m_valueTextStartPosition.y + easedProgress * (m_valueTextEndPosition.y - m_valueTextStartPosition.y);
            m_valueTextOffset = sf::Vector2f(interpolatedX, interpolatedY);
            m_valueTextAnimationTimer = std::min(m_valueTextAnimationTimer, 1.0f);

            sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16 + m_valueTextOffset.x,
                                                 m_gameObject.getPosition().y - m_valueTextOffset.y);
            sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
            m_valueText->setPosition(screenPos.x, screenPos.y);
        }
        else
        {
            m_valueTextOffset = m_valueTextStartPosition;
            sf::Vector2f worldPos = sf::Vector2f(m_gameObject.getPosition().x + 16, m_gameObject.getPosition().y);
            sf::Vector2i screenPos = Game::getGui().getTarget()->mapCoordsToPixel(worldPos, Game::getWindow().getView());
            m_valueText->setPosition(screenPos.x, screenPos.y);
        }
    }

    void PlayerPickUpComponent::addPickUp(GameObject::Ptr pickUp, GameObject::Ptr linkedTrigger)
    {
        if (m_pickUp)
            return;

        ResetInfoText();
        m_infoText->setText("Picked up " + pickUp->getId());

        m_pickUpTimer = 0.0f;
        m_linkedTrigger = linkedTrigger;
        m_linkedTrigger->setActive(false);

        m_pickUp = pickUp;
        m_pickUp->setActive(false);
    }

    void PlayerPickUpComponent::dropPickUp()
    {
        if (!m_pickUp)
            return;

        ResetInfoText();
        m_infoText->setText("Dropped " + m_pickUp->getId());

        m_linkedTrigger->setActive(true);
        m_linkedTrigger = nullptr;

        m_pickUp->setActive(true);
        m_pickUp = nullptr;
    }

    void PlayerPickUpComponent::depositPickUp()
    {
        if (!m_pickUp)
            return;

        m_pickUpTimer = 0.0f;
        auto& item = m_pickUp->getComponent<ItemInfoComponent>();

        ResetInfoText();
        m_infoText->setText("Deposited " + item->getGameObject().getId());
        m_score += item->getValue();

        ResetValueText();
        m_valueText->getRenderer()->setTextColor(tgui::Color(0, 255, 0, 255));
        m_valueText->setText("+ " + std::to_string(item->getValue()));

        m_pickUp = nullptr;
        m_linkedTrigger = nullptr;
    }

    void PlayerPickUpComponent::ResetInfoText()
    {
        m_infoText->setVisible(true);
        m_textDisplayTimer = 0.0f;
        m_textAnimationTimer = 0.0f;
        m_textOffsetY = m_textStartPosition;
    }

    void PlayerPickUpComponent::ResetValueText()
    {
        m_valueText->setVisible(true);
        m_valueTextDisplayTimer = 0.0f;
        m_valueTextAnimationTimer = 0.0f;
        m_valueTextOffset = m_valueTextStartPosition;
    }

    int PlayerPickUpComponent::getPickUpWeight()
    {
        if (!m_pickUp)
            return 0;

        auto& item = m_pickUp->getComponent<ItemInfoComponent>();

        return item->getWeight();
    }

    string PlayerPickUpComponent::getItemName()
    {
        if (!m_pickUp)
            return 0;

        auto& item = m_pickUp->getComponent<ItemInfoComponent>();

        return item->getGameObject().getId();
    }
}
