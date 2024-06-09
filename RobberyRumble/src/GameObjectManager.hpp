/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "EventBus.hpp"
#include "GameObject.hpp"

#include <string>
#include <unordered_map>

namespace mmt_gd
{
    class GameObjectManager
    {
    public:
        static GameObjectManager& getInstance();
        using GameObjectMap = std::unordered_map<std::string, GameObject::Ptr>;

        void init();
        void shutdown();
        void update(float deltaTime);

        void            addGameObject(const GameObject::Ptr& gameObject);
        GameObject::Ptr getGameObject(const std::string& id) const;

        GameObjectMap& getGameObjects()
        {
            return m_gameObjects;
        }

        void removeGameObject(const std::shared_ptr<GameObject>& go);

    private:
        GameObjectManager(void) = default;
        ~GameObjectManager(void) = default;
        GameObjectManager(const GameObjectManager&) = delete;
        GameObjectManager& operator=(const GameObjectManager&) = delete;
        GameObjectMap m_gameObjects;

        std::list<EventBus::ListenerId> m_listeners;
    };
}
