/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once
#include "IComponent.hpp"

namespace mmt_gd
{
    class SpriteRenderComponent;

    class PlayerAnimationComponent : public IComponent
    {
    public:
        enum class AnimationType
        {
            idle,
            walk,
            pickUp,
            punch,
            beBeaten,
            deposit,
        };


        PlayerAnimationComponent(GameObject& gameObject, sf::IntRect texRect);

        bool init() override;
        void update(float deltaTime) override;

        void setAnimation(sf::Vector2f direction, AnimationType animation);


    private:
        enum Direction
		{
			right,
			up,
			left,
			down,
		};

        Direction m_direction = Direction::right;
        AnimationType m_currentAnimation = AnimationType::idle;
        int m_frames = 6;
        int m_nextframes = 6;
        float m_anitmationTime = 0.1f;
        float m_animationTimer = 0.0f;
        int m_currentColumn = 0;
        sf::IntRect m_texRect;
        AnimationType m_nextAnimation = AnimationType::idle;
        int m_frameCounter = 0;
    };
}
