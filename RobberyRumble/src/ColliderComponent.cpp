/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "ColliderComponent.hpp"
#include "DebugDraw.hpp"
#include "EventBus.hpp"
#include "GameObject.hpp"

namespace mmt_gd
{
    ColliderComponent::ColliderComponent(GameObject& gameObject, sf::FloatRect colliderRect) :
    IComponent(gameObject),
    m_colliderRect(colliderRect)
    {
        m_xDiff = m_gameObject.getPosition().x - m_colliderRect.left;
        m_yDiff = m_colliderRect.top - m_gameObject.getPosition().y;
    }

    void ColliderComponent::update(float deltaTime)
    {
        if (!m_gameObject.isActive())
        {
            return;
        }

        m_colliderRect.left = m_gameObject.getPosition().x + m_xDiff;
        m_colliderRect.top = m_gameObject.getPosition().y + m_yDiff;

        DebugDraw::getInstance().drawRectangle(sf::Vector2f(m_colliderRect.left + m_colliderRect.width * 0.5f,
                                                            m_colliderRect.top + m_colliderRect.height * 0.5f),
                                               m_colliderRect.getSize(),
                                               sf::Color::Green);
    }

    void ColliderComponent::registerOnCollisionFunction(const OnCollisionFunction& func)
    {
        m_onCollisionFunctions.push_back(func);
    }

    void ColliderComponent::registerOnTriggerFunction(const OnTriggerFunction& func)
    {
        m_onTriggerFunctions.push_back(func);
    }

    void ColliderComponent::onCollision(ColliderComponent& collider)
    {
        for (const auto& f : m_onCollisionFunctions)
        {
            f(*this, collider);
        }
    }
    void ColliderComponent::onTrigger(ColliderComponent& collider)
    {
        for (const auto& f : m_onTriggerFunctions)
        {
            f(*this, collider);
        }
    }
}
