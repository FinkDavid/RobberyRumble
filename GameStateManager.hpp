/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "GameState.hpp"

#include <memory>
#include <string>
#include <unordered_map>

namespace mmt_gd
{
    class GameStateManager
    {
    public:
        void registerState(const std::string& name, GameState::Ptr state);

        void setState(const std::string& stateName);

        void update(float deltaTime);
        void draw() const;

        void shutdown() const;

    private:
        GameState* findState(const std::string& stateName);
        void       changeState(GameState* stateName);

        std::unordered_map<std::string, GameState::Ptr> m_states;
        GameState*                                      m_currentState = nullptr;
        GameState*                                      m_futureState  = nullptr;
    };
}
