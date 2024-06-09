/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "IController.h"

namespace mmt_gd
{
    class GameObject;

    class HumanControllerComponent : public IController
    {
    public:
        HumanControllerComponent(GameObject& gameObject, int playerIdx);

        void update(float deltaTime) override;

        bool init() override;

        sf::Vector2f getDirection() override;

    private:
        int m_playerIndex;
    };
}
