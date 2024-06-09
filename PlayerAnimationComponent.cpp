/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "GameObject.hpp"
#include "PlayerAnimationComponent.h"
#include "SpriteRenderComponent.hpp"

namespace mmt_gd
{
    PlayerAnimationComponent::PlayerAnimationComponent(GameObject& gameObject, sf::IntRect texRect) :
    IComponent(gameObject),
    m_texRect(texRect)
    {
    }

    bool PlayerAnimationComponent::init()
    {
        return true;
    }

    void PlayerAnimationComponent::setAnimation(sf::Vector2f direction, AnimationType animation)
    {
        if (direction.x > 0)
            m_direction = Direction::right;
        if (direction.x < 0)
            m_direction = Direction::left;
        if (direction.y < 0)
            m_direction = Direction::up;
        if (direction.y > 0)
            m_direction = Direction::down;

        if (animation == AnimationType::beBeaten)
        {
            m_frames = 3;
            m_currentAnimation = animation;
            return;
        }
        else if (animation == AnimationType::punch)
        {
            m_frames = 6;
            m_currentAnimation = animation;
            return;
        }
        else if (animation == AnimationType::pickUp)
        {
            m_nextframes = 12;
        }
        else if (animation == AnimationType::deposit)
        {
            m_nextframes = 10;
        }
        else
        {
            m_nextframes = 6;
        }

        m_nextAnimation = animation;
    }

    void PlayerAnimationComponent::update(float deltaTime)
    {
        m_animationTimer += deltaTime;

        if (m_animationTimer > m_anitmationTime)
        {
            if ((m_currentAnimation == AnimationType::punch || m_currentAnimation == AnimationType::beBeaten) && m_frames != m_frameCounter)
            {
                m_frameCounter++;
            }
            else
            {
                m_currentAnimation = m_nextAnimation;
                m_frames = m_nextframes;
                m_frameCounter = 0;
            }

            m_currentColumn = ((m_currentColumn + 1) % m_frames) + m_frames * m_direction;

            m_texRect.left = m_currentColumn * m_texRect.width;
            m_texRect.top = static_cast<int>(m_currentAnimation) * m_texRect.height;
            auto& spriteComp = m_gameObject.getComponent<SpriteRenderComponent>();
            spriteComp->getSprite().setTextureRect(m_texRect);

            m_animationTimer = 0.f;
        }
    }
}
