/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include <SFML/Window.hpp>

namespace mmt_gd
{
    class ColliderComponent;

    class CollideableManager
    {
    public:
        static CollideableManager& getInstance();
        CollideableManager(const CollideableManager& rhv) = delete;
        CollideableManager(CollideableManager&& rhv) = delete;
        CollideableManager& operator=(const CollideableManager& rhv) = delete;
        CollideableManager&& operator=(CollideableManager&& rhv) = delete;

        std::shared_ptr<ColliderComponent> checkCollision(std::shared_ptr<ColliderComponent> collider);
        std::shared_ptr<ColliderComponent> checkTrigger(std::shared_ptr<ColliderComponent> collider);
        bool checkTrigger2Objects(std::shared_ptr<ColliderComponent> colliderA, std::shared_ptr<ColliderComponent> colliderB);
        std::shared_ptr<ColliderComponent> checkPunchHit(std::shared_ptr<ColliderComponent> collider);

        void addCollider(std::shared_ptr<ColliderComponent> collider);
        void removeCollider(std::shared_ptr<ColliderComponent> collider);

        void shutdown();

    private:
        CollideableManager() = default;
        ~CollideableManager() = default;


        std::vector<std::shared_ptr<ColliderComponent>> m_colliders{};
    };
}
