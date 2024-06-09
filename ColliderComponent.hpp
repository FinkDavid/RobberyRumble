/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once
#include "IComponent.hpp"

#include <functional>
#include <list>

namespace mmt_gd
{


    class ColliderComponent : public IComponent
    {
    public:
        /// \param collider1 the collider for which OnCollision was called.
        /// \param collider2 the collider with which the collision happened.
        using OnCollisionFunction = std::function<void(ColliderComponent&, ColliderComponent&)>;
        using OnTriggerFunction = std::function<void(ColliderComponent&, ColliderComponent&)>;

        /// \param body the rigid body the collider is attached to.
        /// \param rect the size of the AABB in pixel
        ColliderComponent(GameObject& gameObject, sf::FloatRect colliderRect);

        bool init() override
        {
            return true;
        }
        virtual void update(float fDeltaTime) override;
        /// Add delegate function to be executed when collision is detected.
        /// Signature: void func(ColliderComponent&)
        void registerOnCollisionFunction(const OnCollisionFunction& func);
        void registerOnTriggerFunction(const OnTriggerFunction& func);

        ///// Method called when collision occured. Method calls all subscribed
        ///// OnCollisionFunctions
        /////
        ///// \param collider collision occured with this collider
        void onCollision(ColliderComponent& collider);


        void onTrigger(ColliderComponent& collider);

        sf::FloatRect getCollider() const
        {
            return m_colliderRect;
        }

        void setTrigger(bool isTrigger)
        {
            m_isTrigger = isTrigger;
        }

        bool isTrigger()
        {
            return m_isTrigger;
        }

        void setLinkedTrigger(std::string linkedTrigger)
        {
            m_linkedTrigger = linkedTrigger;
        }

        std::string getLinkedTriggerId()
        {
            return m_linkedTrigger;
        }

    private:
        std::list<OnCollisionFunction> m_onCollisionFunctions;
        std::list<OnTriggerFunction> m_onTriggerFunctions;
        sf::FloatRect m_colliderRect;
        bool m_isTrigger = false;
        int m_xDiff;
        int m_yDiff;
        std::string m_linkedTrigger;
    };
} // namespace mmt_gd
