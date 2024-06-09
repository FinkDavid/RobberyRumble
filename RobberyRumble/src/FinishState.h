/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "GameObjectManager.hpp"
#include "GameState.hpp"
#include "SpriteManager.hpp"

#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
    class FINALFRONTIER_API FinishState final : public GameState
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
        tgui::Button::Ptr m_toMainMenuBtn;
        tgui::Button::Ptr m_toGameBtn;
        tgui::Button::Ptr m_endGameBtn;
        tgui::Label::Ptr m_scoreP0;
        tgui::Label::Ptr m_scoreP1;

        sf::Sprite m_finishSprite;
        sf::Image m_finishImage;
        sf::Texture m_finishTexture;
        sf::Vector2u m_windowSize;
        bool m_isInit = false;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindoWsize = 960.0f;
    };
}
