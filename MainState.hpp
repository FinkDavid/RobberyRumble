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
    class FINALFRONTIER_API MainState final : public GameState
    {
    public:
        MainState(GameStateManager* gameStateManager, Game* game);

        void init() override;
        void exit() override;

        void update(float deltaTime) override;
        void draw() override;

    private:
        SpriteManager m_spriteManager;
        std::shared_ptr<GameObject> m_cameraTarget;
        std::shared_ptr<GameObject> m_camera;
        float m_roundTimer = 0.0f;
        float m_roundTime = 120.f;

        float m_preRoundTimer = 0.0f;
        float m_preRoundTime = 3.0f;
        bool m_roundStarted = false;

        float m_timerFontSize = 30.0f;
        tgui::Label::Ptr m_timerText;

        //Responsive UI Variables
        float m_resizeFactor = 1.0f;
        float m_originalWindowSize;
        float m_madeWindowSize = 540.0f;
    };
}
