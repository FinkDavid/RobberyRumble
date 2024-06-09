/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "IComponent.hpp"

#include <list>
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

namespace mmt_gd
{
    class GameObject final : public sf::Transformable
    {
    public:
        using Ptr = std::shared_ptr<GameObject>;

        static Ptr create(const std::string& id)
        {
            return std::make_shared<GameObject>(id);
        }

        explicit GameObject(std::string id) : m_id(std::move(id))
        {
        }

        template <typename T, typename... Args>
        std::shared_ptr<T> addComponent(Args&&... args)
        {
            auto obj = std::make_shared<T>(std::forward<Args>(args)...);
            if (auto cmp = std::dynamic_pointer_cast<IComponent>(obj))
            {
                m_componentList.push_back(cmp);
                return obj;
            }
            return {};
        }

        void removeComponent(const IComponent::Ptr& component);

        template <typename TComponent>
        std::shared_ptr<TComponent> getComponent()
        {
            for (auto& component : m_componentList)
            {
                if (auto cmp = std::dynamic_pointer_cast<TComponent>(component))
                    return cmp;
            }
            return nullptr;
        }

        template <typename TComponent>
        std::vector<std::shared_ptr<TComponent>> getComponents()
        {
            std::vector<std::shared_ptr<TComponent>> result{};
            for (auto& component : m_componentList)
            {
                std::dynamic_pointer_cast<TComponent>(component);
                if (std::shared_ptr<TComponent> cmp = std::dynamic_pointer_cast<TComponent>(component))
                {
                    result.push_back(cmp);
                }
            }
            return result;
        }

        bool init() const;

        void update(float deltaTime) const;

        bool isMarkedForDelete() const
        {
            return m_wantToDie;
        }

        void markForDelete()
        {
            m_wantToDie = true;
        }

        bool isActive() const
        {
            return m_isActive;
        }

        void setActive(const bool isActive)
        {
            m_isActive = isActive;
        }

        std::string getId() const
        {
            return m_id;
        }

        void setId(const std::string& id)
        {
            m_id = id;
        }

        void setTag(const std::string& tag)
        {
            m_tag = tag;
        }

        std::string getTag() const
        {
            return m_tag;
        }

    protected:
        std::string m_id        = "unnamed"; ///< unique name of object, e.g. player
        std::string m_tag       = "unnamed";
        bool        m_wantToDie = false;
        bool        m_isActive  = true;

        std::list<IComponent::Ptr> m_componentList;
    };
}
