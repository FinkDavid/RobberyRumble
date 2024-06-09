/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "GameObject.hpp"
#include "IComponent.hpp"
#include "InputManager.hpp"
#include "ItemInfoComponent.h"

#include <iostream>
#include <cmath>

namespace mmt_gd
{
    class PlayerPickUpComponent : public IComponent
    {
    public:
        PlayerPickUpComponent(GameObject& gameObject, int playerIdx, std::string vanId);

        bool init() override
        {
            return true;
        }

        void update(float deltaTime) override;

        float easeOutQuad(float t)
        {
            return 1.0f - (1.0f - t) * (1.0f - t);
        }

        void addPickUp(GameObject::Ptr pickUp, GameObject::Ptr linkedTrigger);

        void dropPickUp();

        bool hasPickUp() const
        {
            return m_pickUp != nullptr;
        }

        void depositPickUp();

        void ResetInfoText();

        void ResetValueText();

        std::string getVanId()
        {
            return m_vanId;
        }

        int getPickUpWeight();

        string getItemName();

        int getScore() const
        {
			return m_score;
		}

    private:
        int m_playerIndex;
        std::string m_vanId;
        GameObject::Ptr m_pickUp;
        GameObject::Ptr m_linkedTrigger;
        int m_score = 0;
        float m_pickUpTime = 10.0f;
        float m_pickUpTimer = 0.0f;
        float m_elapsedTime = 0.0f;

        //InfoText
        tgui::Label::Ptr m_infoText;
        float m_infoTextSize = 10.0f;
        float m_textStartPosition = -25.0f;
        float m_textEndPosition = 0.0f;
        float m_textOffsetY = 0.0f;
        float m_textDisplayTimer = 0.0f;
        float m_textAnimationTimer = 0.0f;
        float m_textDisplayTime = 2.0f;

        //ItemValue Text
        tgui::Label::Ptr m_valueText;
        float m_valueTextSize = 8.0f;
        sf::Vector2f m_valueTextStartPosition = sf::Vector2f(10.0f, -25.0f);
        sf::Vector2f m_valueTextEndPosition = sf::Vector2f(35.0f, -10.0f);
        sf::Vector2f m_valueTextOffset = sf::Vector2f(0.0f, 0.0f);
        float m_valueTextDisplayTimer = 0.0f;
        float m_valueTextAnimationTimer = 0.0f;
        float m_valueTextDisplayTime = 0.9f;

        //Responsive UI Variables
        float m_originalWindowSize;
        float m_madeWindowSize = 540.0f;
        float m_resizeFactor = 1.0f;
    };
}
