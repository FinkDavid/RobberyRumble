/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "CameraRenderComponent.hpp"

#include "GameObject.hpp"

namespace mmt_gd
{
    CameraRenderComponent::CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, sf::View view) :
    IRenderComponent(gameObject, renderWindow),
    m_view(view)
    {

    }

    void CameraRenderComponent::autoZoomCamera(sf::Vector2f p0, sf::Vector2f p1)
    {
        float distanceX = std::abs(p0.x - p1.x);
        float distanceY = std::abs(p0.y - p1.y);

        float targetViewSizeX = distanceX * 1.5f;
        float targetViewSizeY = distanceY * 1.5f;

        if (targetViewSizeX < m_minZoom)
            targetViewSizeX = m_minZoom;

        if (targetViewSizeY < m_minZoom)
            targetViewSizeY = m_minZoom;

        float aspectRatio = m_view.getSize().x / m_view.getSize().y;
        float targetViewSize = std::max(targetViewSizeX, targetViewSizeY * aspectRatio);

        float interpolationFactor = 0.1f;

        sf::Vector2f currentViewSize = m_view.getSize();
        float newViewSizeX = currentViewSize.x + (targetViewSize - currentViewSize.x) * interpolationFactor;
        float newViewSizeY = newViewSizeX / aspectRatio;

        m_view.setSize(newViewSizeX, newViewSizeY);
    }


    bool CameraRenderComponent::init()
    {
        return true;
    }

    void CameraRenderComponent::draw()
    {

    }

    void CameraRenderComponent::update(float deltaTime)
    {
        m_view.setCenter(m_gameObject.getPosition());
        m_view = getLetterboxView(m_view, m_renderWindow.getSize().x, m_renderWindow.getSize().y);
        m_renderWindow.setView(m_view);
    }

    sf::View CameraRenderComponent::getLetterboxView(sf::View view, int windowWidth, int windowHeight)
    {
        float windowRatio = windowWidth / static_cast<float>(windowHeight);
        float viewRatio = view.getSize().x / view.getSize().y;

        float sizeX = 1.0f;
        float sizeY = 1.0f;
        float posX = 0.0f;
        float posY = 0.0f;

        bool horizontalSpacing = true;

        if (windowRatio < viewRatio)
        {
            horizontalSpacing = false;
            sizeX = viewRatio / windowRatio;
            posX = (1.0f - sizeX) / 2.0f;
        }
        else
        {
            sizeY = windowRatio / viewRatio;
            posY = (1.0f - sizeY) / 2.0f;
        }

        view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

        return view;
    }
}
