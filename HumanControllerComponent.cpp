/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "GameObject.hpp"
#include "HumanControllerComponent.h"
#include "InputManager.hpp"
#include "VectorAlgebra2D.h"

#include <SFML/Window/Joystick.hpp>

namespace mmt_gd
{
    HumanControllerComponent::HumanControllerComponent(GameObject& gameObject, int playerIdx) :
    IController(gameObject),
    m_playerIndex(playerIdx)
    {

    }

    void HumanControllerComponent::update(float deltaTime)
    {

    }

    bool HumanControllerComponent::init()
    {
        return true;
    }

    sf::Vector2f HumanControllerComponent::getDirection()
    {
        sf::Vector2f translation{};

        //Controller Movement
        float xAxis = sf::Joystick::getAxisPosition(m_playerIndex, sf::Joystick::X);
        float yAxis = sf::Joystick::getAxisPosition(m_playerIndex, sf::Joystick::Y);

        if (std::abs(xAxis) > 20.0f)
        {
            translation.x += xAxis;
        }

        if (std::abs(yAxis) > 20.0f)
        {
            translation.y += yAxis;
        }

        //Keyboard Movement
        if (InputManager::getInstance().isKeyDown("right", m_playerIndex))
        {
            translation.x = 1.0f;
        }
        if (InputManager::getInstance().isKeyDown("left", m_playerIndex))
        {
            translation.x = -1.0f;
        }
        if (InputManager::getInstance().isKeyDown("up", m_playerIndex))
        {
            translation.y = -1.0f;
        }
        if (InputManager::getInstance().isKeyDown("down", m_playerIndex))
        {
            translation.y = 1.0f;
        }

        if (translation == sf::Vector2f{0, 0})
        {
            return translation;
        }

        return MathUtil::unitVector(translation);
    };
}
