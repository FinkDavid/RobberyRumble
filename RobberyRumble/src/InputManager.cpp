/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "InputManager.hpp"

namespace mmt_gd
{
    InputManager& InputManager::getInstance()
    {
        static InputManager instance;
        return instance;
    }

    void InputManager::process(const sf::Event& event)
    {
        switch (event.type)
        {
            case sf::Event::KeyPressed:
                m_eventFrame.m_keys[event.key.code] = true;
                break;
            case sf::Event::JoystickButtonPressed:
                m_eventFrame.m_controllerButtons[event.joystickButton.joystickId][event.joystickButton.button] = true;
                break;
            case sf::Event::KeyReleased:
                m_eventFrame.m_keys[event.key.code] = false;
                break;
            case sf::Event::JoystickButtonReleased:
                m_eventFrame.m_controllerButtons[event.joystickButton.joystickId][event.joystickButton.button] = false;
                break;
            default:
                break;
        }
    }

    void InputManager::init()
    {
        sf::Joystick::update();

        bind("Exit", sf::Keyboard::Escape);
        bind("Select", sf::Keyboard::Space);

        bindController("handlePickup", 0, 0);
        bindController("dropPickUp", 2, 0);
        bindController("punch", 1, 0);

        bindController("handlePickup", 0, 1);
        bindController("dropPickUp", 2, 1);
        bindController("punch", 1, 1);

        bind("up", sf::Keyboard::W, 0);
        bind("left", sf::Keyboard::A, 0);
        bind("down", sf::Keyboard::S, 0);
        bind("right", sf::Keyboard::D, 0);
        bind("handlePickup", sf::Keyboard::Space, 0);
        bind("dropPickUp", sf::Keyboard::R, 0);
        bind("punch", sf::Keyboard::E, 0);

        bind("up", sf::Keyboard::Up, 1);
        bind("left", sf::Keyboard::Left, 1);
        bind("down", sf::Keyboard::Down, 1);
        bind("right", sf::Keyboard::Right, 1);
        bind("handlePickup", sf::Keyboard::Numpad0, 1);
        bind("dropPickUp", sf::Keyboard::Numpad1, 1);
        bind("punch", sf::Keyboard::RShift, 1);

        bind("debugdraw", sf::Keyboard::F1, 0);
    }

    void InputManager::update()
    {
        m_lastFrame    = m_currentFrame;
        m_currentFrame = m_eventFrame;
    }

    void InputManager::bind(const std::string& action, const int keyCode, const int playerIdx)
    {
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx][action] = keyCode;
    }

    void InputManager::bindController(const std::string& action, const int controllerCode, const int playerIdx)
    {
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_controllerBinding[playerIdx][action] = controllerCode;
    }

    void InputManager::unbind(const std::string& action, const int playerIdx)
    {
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_actionBinding[playerIdx].erase(action);
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds") m_controllerBinding[playerIdx].erase(action);
    }

    bool InputManager::isKeyDown(const int keyCode) const
    {
        if (keyCode >= 0 && keyCode < sf::Keyboard::KeyCount)
        {
            return m_currentFrame.m_keys[keyCode];
        }

        return false;
    }

    bool InputManager::isButtonDown(const int buttonCode, const int playerIdx) const
    {
        if (buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount)
        {
            return m_currentFrame.m_controllerButtons[playerIdx][buttonCode];
        }

        return false;
    }

    bool InputManager::isKeyUp(const int keyCode) const
    {
        if (keyCode >= 0 && keyCode < sf::Keyboard::KeyCount)
        {
            return !m_currentFrame.m_keys[keyCode];
        }

        return false;
    }

    bool InputManager::isButtonUp(const int buttonCode, const int playerIdx) const
    {
        if (buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount)
        {
            return !m_currentFrame.m_controllerButtons[playerIdx][buttonCode];
        }

        return false;
    }

    bool InputManager::isKeyPressed(const int keyCode) const
    {
        if (keyCode >= 0 && keyCode < sf::Keyboard::KeyCount)
        {
            return m_currentFrame.m_keys[keyCode] && !m_lastFrame.m_keys[keyCode];
        }

        return false;
    }

    bool InputManager::isButtonPressed(const int buttonCode, const int playerIdx) const
    {
        if (buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount)
        {
            return m_currentFrame.m_controllerButtons[playerIdx][buttonCode] &&
                   !m_lastFrame.m_controllerButtons[playerIdx][buttonCode];
        }

        return false;
    }

    bool InputManager::isKeyReleased(const int key_code) const
    {
        if (key_code >= 0 && key_code < sf::Keyboard::KeyCount)
        {
            return !m_currentFrame.m_keys[key_code] && m_lastFrame.m_keys[key_code];
        }

        return false;
    }

    bool InputManager::isButtonReleased(const int buttonCode, const int playerIdx) const
    {
        if (buttonCode >= 0 && buttonCode < sf::Joystick::ButtonCount)
        {
            return !m_currentFrame.m_controllerButtons[playerIdx][buttonCode] &&
                   m_lastFrame.m_controllerButtons[playerIdx][buttonCode];
        }

        return false;
    }

    int InputManager::getKeyForAction(const std::string& action, const int playerIdx)
    {
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        const auto it = m_actionBinding[playerIdx].find(action);
        if (it != m_actionBinding[playerIdx].end())
        {
            return it->second;
        }
        return -1;
    }

    int InputManager::getButtonForAction(const std::string& action, const int playerIdx)
    {
        ffAssertMsg(playerIdx < PlayerCount, "player out of bounds")

        const auto it = m_controllerBinding[playerIdx].find(action);
        if (it != m_controllerBinding[playerIdx].end())
        {
            return it->second;
        }
        return -1;
    }

    bool InputManager::isKeyDown(const std::string& action, const int playerIdx)
    {
        return isKeyDown(getKeyForAction(action, playerIdx)) || isButtonDown(getButtonForAction(action, playerIdx), playerIdx);
    }

    bool InputManager::isKeyUp(const std::string& action, const int playerIdx)
    {
        return isKeyUp(getKeyForAction(action, playerIdx)) || isButtonUp(getButtonForAction(action, playerIdx), playerIdx);
    }

    bool InputManager::isKeyPressed(const std::string& action, const int playerIdx)
    {
        return isKeyPressed(getKeyForAction(action, playerIdx)) || isButtonPressed(getButtonForAction(action, playerIdx), playerIdx);
    }

    bool InputManager::isKeyReleased(const std::string& action, const int playerIdx)
    {
        return isKeyReleased(getKeyForAction(action, playerIdx)) || isButtonReleased(getButtonForAction(action, playerIdx), playerIdx);
    }

    sf::Vector2f InputManager::getMousePosition() const
    {
        ffAssertMsg(m_renderWindow != nullptr, "RenderWindow not set for getMousePosition.")

            const auto p = sf::Mouse::getPosition(*m_renderWindow);
        return {static_cast<float>(p.x), static_cast<float>(p.y)};
    }
}
