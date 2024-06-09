/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "CollideableManager.h"
#include "ColliderComponent.hpp"
#include "GameObject.hpp"

namespace mmt_gd
{
    CollideableManager& CollideableManager::getInstance()
    {
        static CollideableManager instance;
        return instance;
    }

    std::shared_ptr<ColliderComponent> CollideableManager::checkCollision(std::shared_ptr<ColliderComponent> collider)
    {
        for (auto& otherCollider : m_colliders)
        {
            if (!otherCollider->getGameObject().isActive())
                continue;

            if (collider == otherCollider)
                continue;

            if (otherCollider->isTrigger())
                continue;

            if (collider->getCollider().intersects(otherCollider->getCollider()))
            {
                otherCollider->onCollision(*collider);

                return otherCollider;
            }
        }

        return nullptr;
    }

    std::shared_ptr<ColliderComponent> CollideableManager::checkTrigger(std::shared_ptr<ColliderComponent> collider)
    {
        for (auto& otherCollider : m_colliders)
        {
            if (!otherCollider->getGameObject().isActive())
                continue;

            if (collider == otherCollider)
                continue;

            if (collider->getCollider().intersects(otherCollider->getCollider()) && otherCollider->isTrigger())
            {
                if (otherCollider->getGameObject().getId() != collider->getGameObject().getId())
                {
                    return otherCollider;
                }
            }
        }
        return nullptr;
    }

    bool CollideableManager::checkTrigger2Objects(std::shared_ptr<ColliderComponent> colliderA,
                                                  std::shared_ptr<ColliderComponent> colliderB)
    {
        if (colliderA->getCollider().intersects(colliderB->getCollider()))
        {
            return true;
        }

        return false;
    }

    
    std::shared_ptr<ColliderComponent> CollideableManager::checkPunchHit(std::shared_ptr<ColliderComponent> collider)
    {
        for (auto& otherCollider : m_colliders)
        {
            if (otherCollider->getGameObject().getTag() == "Player" && otherCollider->getGameObject().getId() != collider->getGameObject().getId())
            {
                if (collider->getCollider().intersects(otherCollider->getCollider()) && otherCollider->isTrigger())
                {
                    if (otherCollider->getGameObject().getId() != collider->getGameObject().getId())
                    {
                        return otherCollider;
                    }
                }
            }
        }

        return nullptr;
    }

    void CollideableManager::addCollider(std::shared_ptr<ColliderComponent> collider)
    {
        m_colliders.push_back(collider);
    }

    void CollideableManager::removeCollider(std::shared_ptr<ColliderComponent> collider)
    {
        m_colliders.erase(std::remove(m_colliders.begin(), m_colliders.end(), collider), m_colliders.end());
    }

    void CollideableManager::shutdown()
    {
        m_colliders.clear();
    }
}
