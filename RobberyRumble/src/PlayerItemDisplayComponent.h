/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "AssetManager.h"
#include "Game.hpp"
#include "GameObject.hpp"
#include "IComponent.hpp"

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
    class PlayerItemDisplayComponent : public IComponent
    {
    public:
        PlayerItemDisplayComponent(GameObject& gameObject, int playerIdx);

        bool init() override
        {
            return true;
        }

        void update(float deltaTime) override;

        void updateIcon(Texture texture);

        void showIcon(bool isVisible);

        void setView(sf::View view);

    private:
        int m_playerIndex;
        float m_iconSize = 32.0f;
        tgui::Texture m_iconTexture;
        tgui::Picture::Ptr m_icon;
        sf::View m_view;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindowSize = 540.0f;
    };
}
