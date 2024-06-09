/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "AssetManager.h"
#include "Game.hpp"
#include "GameObjectManager.hpp"
#include "IComponent.hpp"

#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
	class ItemInfoComponent : public IComponent
    {
    public:
        ItemInfoComponent(GameObject& gameObject, int m_value, int m_weight, sf::Vector2f textPos);

        bool init() override
        {
            return true;
        }

        void update(float deltaTime) override;

        void ShowPickUpText(bool isVisible)
        {
            m_infoText->setVisible(isVisible);
        }

        int getValue() const
        {
			return m_value;
		}

        int getWeight() const
        {
            return m_weight;
        }

    private:
        int m_value;
        int m_weight;

        //Info Text Variables
        int m_fontSize = 10;
        sf::Vector2f m_textPosition;
        tgui::Label::Ptr m_infoText;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindowSize = 540.0f;
    };
}
