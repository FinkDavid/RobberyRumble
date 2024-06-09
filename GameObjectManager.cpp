/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "Debug.hpp"
#include "GameObjectEvents.hpp"
#include "GameObjectManager.hpp"

namespace mmt_gd
{
    GameObjectManager& GameObjectManager::getInstance()
    {
        static GameObjectManager gameObjectManager;
        return gameObjectManager;
    }

    void GameObjectManager::init()
    {
        {
            const EventBus::ListenerId
                id = EventBus::getInstance()
                         .addListener(GameObjectCreateEvent::Type,
                                      [this](const IEvent::Ptr& event)
                                      {
                                          const auto goCreateEvent = std::static_pointer_cast<GameObjectCreateEvent>(event);
                                          this->addGameObject(goCreateEvent->getData());
                                      });
            m_listeners.push_back(id);
        }
    }

    void GameObjectManager::shutdown()
    {
        m_gameObjects.clear();

        for (const auto& listener : m_listeners)
        {
            EventBus::getInstance().removeListener(listener);
        }
        m_listeners.clear();
    }

    void GameObjectManager::update(const float deltaTime)
    {
        std::vector<GameObject::Ptr> gameObjectsToDelete{};

        for (const auto& goPair : getGameObjects())
        {
            if (goPair.second->isMarkedForDelete())
            {
                gameObjectsToDelete.push_back(goPair.second);
            }
            else if (goPair.second->isActive())
            {
                goPair.second->update(deltaTime);
            }
        }
        for (const auto& go : gameObjectsToDelete)
        {
            removeGameObject(go);
        }
    }

    void GameObjectManager::addGameObject(const GameObject::Ptr& gameObject)
    {
        ffAssertMsg(m_gameObjects.find(gameObject->getId()) == m_gameObjects.end(),
                    "Game object with this m_id already exists " + gameObject->getId())

            m_gameObjects[gameObject->getId()] = gameObject;
    }

    GameObject::Ptr GameObjectManager::getGameObject(const std::string& id) const
    {
        const auto it = m_gameObjects.find(id);
        if (it == m_gameObjects.end())
        {
            ffErrorMsg("Could not find gameobject with m_id " + id) return nullptr;
        }
        return it->second;
    }

    void GameObjectManager::removeGameObject(const std::shared_ptr<GameObject>& go)
    {
        if (m_gameObjects.find(go->getId()) != m_gameObjects.end())
        {
            m_gameObjects.erase(go->getId());
        }
    }
}
