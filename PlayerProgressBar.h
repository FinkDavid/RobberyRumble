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
    class PlayerProgressBar : public IComponent
    {
    public:
        PlayerProgressBar(GameObject& gameObject, int playerIdx);

        bool init() override
        {
            return true;
        }

        void update(float deltaTime) override;

        void updateProgressBar(float value);

        void showProgressBar(bool isVisible);

        void setProgressBarColor(tgui::Color color);

        void setMaxMinValue(float max, float min);

        void setView(sf::View view);

    private:
        int m_playerIndex;
        tgui::ProgressBar::Ptr m_progressBar;
        sf::Vector2f m_progressBarSize = sf::Vector2f(20.0f, 5.0f);
        sf::View m_view;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindowSize = 540.0f;
    };
}
