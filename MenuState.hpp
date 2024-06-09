/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "GameState.hpp"

#include <SFML/Graphics/Text.hpp>

namespace mmt_gd
{
    class MenuState final : public GameState
    {
    public:
        using GameState::GameState;

        void init() override;

        void update(float delta) override;
        void draw() override;
        void exit() override;

    private:
        sf::Text m_text;
        sf::Font m_font;
        sf::View m_view;

        tgui::Button::Ptr m_startButton;
        tgui::Button::Ptr m_exitButton;
        tgui::Label::Ptr m_creditsText;
        sf::Sprite m_backgroundSprite;
        sf::Image m_backgroundImage;
        sf::Texture m_backgroundTexture;

        bool m_isInit = false;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindoWsize = 960.0f;
    };
}
