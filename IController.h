/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "IComponent.hpp"

namespace mmt_gd
{

class IController : public IComponent
{
public:
    using Ptr = std::shared_ptr<IController>;

    IController(GameObject& gameObject) : IComponent(gameObject)
    {

    }

    virtual sf::Vector2f getDirection() = 0;

    void update(float deltaTime) override
    {
    }

    bool init() override
    {
        return true;
    }
};

} // namespace mmt_gd
