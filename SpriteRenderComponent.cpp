/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "EventBus.hpp"
#include "GameObject.hpp"
#include "RenderComponentEvents.hpp"
#include "SpriteRenderComponent.hpp"

#include <utility>

namespace mmt_gd
{
    SpriteRenderComponent::SpriteRenderComponent(GameObject&       gameObject,
                                                 sf::RenderWindow& renderWindow,
                                                 std::string       textureFile,
                                                 std::string       layerName) :
    IRenderComponent(gameObject, renderWindow),
    m_textureFile(std::move(textureFile)),
    m_layerName(std::move(layerName))
    {
        EventBus::getInstance().fireEvent(std::make_shared<RenderableCreateEvent>(m_layerName, *this));
    }

    SpriteRenderComponent::~SpriteRenderComponent()
    {
        EventBus::getInstance().fireEvent(std::make_shared<RenderableDestroyEvent>(m_layerName, *this));
    }

    bool SpriteRenderComponent::init()
    {
        sf::Image image;
        if (!image.loadFromFile(m_textureFile))
        {
            sf::err() << "Could not load texture from " << m_textureFile << std::endl;
            return false;
        }
        image.createMaskFromColor(sf::Color::Black);
        m_texture.loadFromImage(image);
        m_sprite.setTexture(m_texture);

        return true;
    }

    void SpriteRenderComponent::draw()
    {
        m_renderWindow.draw(m_sprite, m_gameObject.getTransform());
    }
}
