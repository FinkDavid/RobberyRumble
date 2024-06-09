/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include <SFML/Window.hpp>
#include <unordered_map>

namespace mmt_gd
{
    class InputManager
    {
    public:
        static InputManager& getInstance();
        InputManager(const InputManager& rhv)             = delete;
        InputManager(InputManager&& rhv)                  = delete;
        InputManager&  operator=(const InputManager& rhv) = delete;
        InputManager&& operator=(InputManager&& rhv)      = delete;

        void process(const sf::Event& event);

        void init();
        void update();

        void bind(const std::string& action, int keyCode, int playerIdx = 0);
        void bindController(const std::string& action, const int controllerCode, const int playerIdx);

        void unbind(const std::string& action, int playerIdx = 0);

        bool isKeyDown(const std::string& action, int playerIdx = 0);
        bool isKeyUp(const std::string& action, int playerIdx = 0);
        bool isKeyPressed(const std::string& action, int playerIdx = 0);
        bool isKeyReleased(const std::string& action, int playerIdx = 0);

        //Keyboard Checks
        bool isKeyDown(const int keyCode) const;
        bool isKeyUp(const int keyCode) const;
        bool isKeyPressed(const int keyCode) const;
        bool isKeyReleased(const int key_code) const;

        //Controller Checks
        bool isButtonDown(const int buttonCode, const int playerIdx) const;
        bool isButtonUp(const int buttonCode, const int playerIdx) const;
        bool isButtonPressed(const int buttonCode, const int playerIdx) const;
        bool isButtonReleased(const int buttonCode, const int playerIdx) const;

        sf::Vector2f getMousePosition() const;

        void setRenderWindow(sf::RenderWindow* window)
        {
            m_renderWindow = window;
        }

    private:
        InputManager()  = default;
        ~InputManager() = default;


        int getKeyForAction(const std::string& action, int playerIdx);
        int getButtonForAction(const std::string& action, int playerIdx);

        static constexpr int PlayerCount = 2;

        struct FrameData
        {
            bool m_keys[sf::Keyboard::KeyCount];
            bool m_controllerButtons[PlayerCount][sf::Joystick::ButtonCount];
        };

        FrameData m_lastFrame{};
        FrameData m_currentFrame{};
        FrameData m_eventFrame{};

        sf::RenderWindow* m_renderWindow{nullptr};

        std::unordered_map<std::string, int> m_actionBinding[PlayerCount];
        std::unordered_map<std::string, int> m_controllerBinding[PlayerCount];
    };
}
