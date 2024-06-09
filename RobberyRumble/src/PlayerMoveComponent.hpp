/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "IComponent.hpp"
#include "IController.h"

namespace mmt_gd
{
    class ColliderComponent;
    class PlayerAnimationComponent;

    class PlayerMoveComponent : public IComponent
    {
    public:
        using ptr = std::shared_ptr<PlayerMoveComponent>;

        PlayerMoveComponent(GameObject& gameObject, IController::Ptr controller, int playerIdx);

        void increaseSpeed(float value);

        bool init() override;
        void update(float deltaTime) override;

        void setCanMove(bool canMove);

        bool isOnDeposit()
        {
            return m_isOnDeposit;
        }

    private:
        IController::Ptr m_controller;

        sf::Vector2f m_prevPosition;

        int m_playerIndex;
        float m_speed = 175.f;
        float m_pickUpTime = 2.f;
        float m_pickUpTimer = 0.f;
        const float m_slowDownFactor = 20.f;
        bool m_enteredTrigger = false;
        bool m_collissionSolveDesider = false;
        int m_collissionSolveCtr = 0;
        const float m_punchCooldown = 5.f;
        float m_punchTimer = 5.f;
        sf::Vector2f m_direction{};
        std::shared_ptr<GameObject> m_triggeredItem;
        std::shared_ptr<GameObject> m_triggeredItemTrigger;
        bool m_canMove = false;
        bool m_isOnDeposit = false;

        void showCollectable(std::shared_ptr<ColliderComponent> trigger,
                             std::shared_ptr<ColliderComponent> playerCollider,
                             std::string triggerTag);

        void movePlayer(float deltaTime, std::shared_ptr<PlayerAnimationComponent> animation, int weight);

        void showProgressBar(float deltaTime, tgui::Color color);
    };
}
