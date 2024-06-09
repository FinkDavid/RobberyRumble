/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "PlayerMoveComponent.hpp"

#include "CollideableManager.h"
#include "ColliderComponent.hpp"
#include "GameObject.hpp"
#include "GameObjectManager.hpp"
#include "InputManager.hpp"
#include "PlayerAnimationComponent.h"
#include "PlayerItemDisplayComponent.h"
#include "PlayerPickUpComponent.h"
#include "PlayerProgressBar.h"
#include "TransformAnimationConstantMotion.hpp"

#include <TGUI/TGUI.hpp>

namespace mmt_gd
{
    PlayerMoveComponent::PlayerMoveComponent(GameObject& gameObject, IController::Ptr controller, int playerIdx) :
    IComponent(gameObject),
    m_controller(controller),
    m_prevPosition(gameObject.getPosition()),
    m_playerIndex(playerIdx)
    {
        m_gameObject.addComponent<TransformAnimationConstantMotion>(sf::Vector2f(1.f, 0.f), m_speed);
    }

    void PlayerMoveComponent::increaseSpeed(float value)
    {
        m_speed += value;
    }

    bool PlayerMoveComponent::init()
    {
        return true;
    }

    void PlayerMoveComponent::update(const float deltaTime)
    {
        m_punchTimer += deltaTime;
        m_direction = m_controller->getDirection();

        auto colliders = m_gameObject.getComponents<ColliderComponent>();

        auto& collider = colliders.at(0);
        auto& playerTrigger = colliders.at(1);

        auto collision = CollideableManager::getInstance().checkCollision(collider);

        auto& animation = m_gameObject.getComponent<PlayerAnimationComponent>();

        auto& trigger = CollideableManager::getInstance().checkTrigger(collider);

        auto triggerTag = trigger ? trigger->getGameObject().getTag() : "";

        auto& playerTriggerCollision = CollideableManager::getInstance().checkTrigger(playerTrigger);

        auto enemyTriggerTag = playerTriggerCollision ? playerTriggerCollision->getGameObject().getTag() : "";

        auto& pickUpComponent = m_gameObject.getComponent<PlayerPickUpComponent>();


        showCollectable(trigger, collider, triggerTag);

        // reset depoit state
        if (InputManager::getInstance().isKeyUp("handlePickup", m_playerIndex))
            m_isOnDeposit = false;

        // drop pickup
        if (pickUpComponent->hasPickUp() && InputManager::getInstance().isKeyDown("dropPickUp", m_playerIndex))
        {
            showProgressBar(deltaTime, tgui::Color(255, 0, 0, 255));

            animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::deposit);

            // time to drop up item
            if (m_pickUpTimer > m_pickUpTime)
            {
                m_gameObject.getComponent<PlayerItemDisplayComponent>()->showIcon(false);
                m_gameObject.getComponent<PlayerProgressBar>()->showProgressBar(false);
                m_pickUpTimer = 0.f;
                pickUpComponent->dropPickUp();
                AssetManager::getInstance().playSound("drop");
                animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::idle);
            }
        }
        // punch
        else if (m_punchTimer >= m_punchCooldown && !pickUpComponent->hasPickUp() && InputManager::getInstance().isKeyPressed("punch", m_playerIndex))
        {
            m_punchTimer = 0.f;
            AssetManager::getInstance().playSound("hit");

            animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::punch);
            if (auto& otherPlayer = CollideableManager::getInstance().checkPunchHit(collider))
            {
                auto& playerMoveComponent = otherPlayer->getGameObject().getComponent<PlayerMoveComponent>();
                if (playerMoveComponent->isOnDeposit())
                    return;
                otherPlayer->getGameObject()
                    .getComponent<PlayerAnimationComponent>()
                    ->setAnimation(sf::Vector2f{}, PlayerAnimationComponent::AnimationType::beBeaten);
                otherPlayer->getGameObject().getComponent<PlayerPickUpComponent>()->dropPickUp();
                otherPlayer->getGameObject().getComponent<PlayerItemDisplayComponent>()->showIcon(false);
            }
        }
        // handle pickup
        else if (InputManager::getInstance().isKeyDown("handlePickup", m_playerIndex) && trigger)
        {
            // deposit pickup
            if (pickUpComponent->hasPickUp() && triggerTag == "DepositTrigger" &&
                pickUpComponent->getVanId() == trigger->getGameObject().getId())
            {
                m_isOnDeposit = true;
                showProgressBar(deltaTime, tgui::Color(0, 0, 255, 255));

                animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::deposit);

                // time to deposit item
                if (m_pickUpTimer > m_pickUpTime)
                {
                    m_gameObject.getComponent<PlayerProgressBar>()->showProgressBar(false);
                    m_pickUpTimer = 0.f;
                    trigger->onTrigger(*collider);
                    AssetManager::getInstance().playSound("deposit");
                    animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::idle);
                }
            }
            // pick up pickup
            else if (!pickUpComponent->hasPickUp() && triggerTag == "Trigger")
            {
                showProgressBar(deltaTime, tgui::Color(0, 0, 255, 255));

                animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::pickUp);

                // time to pick up item
                if (m_pickUpTimer > m_pickUpTime)
                {
                    AssetManager::getInstance().playSound("collect");
                    m_gameObject.getComponent<PlayerProgressBar>()->showProgressBar(false);
                    m_pickUpTimer = 0.f;
                    trigger->onTrigger(*collider);
                    m_triggeredItem->getComponent<ItemInfoComponent>()->ShowPickUpText(false);
                    m_enteredTrigger = false;
                    m_triggeredItem = nullptr;
                    m_triggeredItemTrigger = nullptr;
                    animation->setAnimation(m_direction, PlayerAnimationComponent::AnimationType::idle);
                }
            }
            // walk
            else if (!collision)
            {
                movePlayer(deltaTime, animation, pickUpComponent->getPickUpWeight());
            }
        }

        // walk
        else if (!collision)
        {
            movePlayer(deltaTime, animation, pickUpComponent->getPickUpWeight());
        }
        // collision
        else
        {
            sf::Vector2f newPos = collider->getGameObject().getPosition();

            auto collisionDirection = newPos - m_prevPosition;

            if (newPos == m_prevPosition)
            {
                collision->getGameObject().move(m_direction);
			}

            // desicicion to solve collision on x or y axis first
            // if x and then y not possible, then y and then x
            // to avoid getting stuck in diagonal collision
            if (m_collissionSolveDesider)
            {
                if (collisionDirection.x != 0.f)
                    newPos.x = m_prevPosition.x;
                else if (collisionDirection.y != 0.f)
                    newPos.y = m_prevPosition.y;
            }
            else
            {
                if (collisionDirection.y != 0.f)
                    newPos.y = m_prevPosition.y;
                else if (collisionDirection.x != 0.f)
                    newPos.x = m_prevPosition.x;
            }

            m_gameObject.setPosition(newPos);
            m_pickUpTimer = 0.f;

            m_collissionSolveCtr++;
            m_collissionSolveCtr = m_collissionSolveCtr % 2;
            if (m_collissionSolveCtr == 0)
            {
                m_collissionSolveDesider = !m_collissionSolveDesider;
            }
        }

    }

    void PlayerMoveComponent::setCanMove(bool canMove)
    {
        m_canMove = canMove;
    }

    void PlayerMoveComponent::showCollectable(std::shared_ptr<ColliderComponent> trigger,
                                              std::shared_ptr<ColliderComponent> playerCollider,
                                              std::string triggerTag)
    {
        if (trigger && !m_enteredTrigger)
        {
            if (triggerTag == "Trigger")
            {
                m_enteredTrigger = true;
                m_triggeredItemTrigger = std::make_shared<GameObject>(trigger->getGameObject());
                m_triggeredItem = GameObjectManager::getInstance().getGameObject(trigger->getLinkedTriggerId());
                m_triggeredItem->getComponent<ItemInfoComponent>()->ShowPickUpText(true);
            }
        }

        if (m_triggeredItem != nullptr && m_enteredTrigger)
        {
            if (CollideableManager::getInstance()
                    .checkTrigger2Objects(m_triggeredItemTrigger->getComponent<ColliderComponent>(), playerCollider) == false)
            {
                m_enteredTrigger = false;
                m_triggeredItem->getComponent<ItemInfoComponent>()->ShowPickUpText(false);
            }
        }
    }

    void PlayerMoveComponent::movePlayer(float deltaTime, std::shared_ptr<PlayerAnimationComponent> animation, int weight)
    {
        if (!m_canMove)
        {
            return;
        }
        m_prevPosition = m_gameObject.getPosition();

        m_gameObject.setPosition((m_gameObject.getPosition().x +
                                  m_direction.x * deltaTime * (m_speed - weight * m_slowDownFactor)),
                                 m_gameObject.getPosition().y +
                                     m_direction.y * deltaTime * (m_speed - weight * m_slowDownFactor));

        auto animationType = m_direction == sf::Vector2f(0.f, 0.f) ? PlayerAnimationComponent::AnimationType::idle
                                                                   : PlayerAnimationComponent::AnimationType::walk;

        animation->setAnimation(m_direction, animationType);

        m_pickUpTimer = 0.f;
        m_gameObject.getComponent<PlayerProgressBar>()->showProgressBar(false);
    }


    void PlayerMoveComponent::showProgressBar(float deltaTime, tgui::Color color)
    {
        m_pickUpTimer += deltaTime;
        m_gameObject.getComponent<PlayerProgressBar>()->setProgressBarColor(color);
        m_gameObject.getComponent<PlayerProgressBar>()->setMaxMinValue(m_pickUpTime, 0.0f);
        m_gameObject.getComponent<PlayerProgressBar>()->showProgressBar(true);
        m_gameObject.getComponent<PlayerProgressBar>()->updateProgressBar(m_pickUpTimer);
    }
}
