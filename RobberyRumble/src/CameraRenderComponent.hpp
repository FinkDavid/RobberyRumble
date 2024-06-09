/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once

#include "IRenderComponent.hpp"

#include <SFML/Graphics.hpp>

namespace mmt_gd
{
class CameraRenderComponent final : public IRenderComponent
{
    public:
        using Ptr = std::shared_ptr<CameraRenderComponent>;

        CameraRenderComponent(GameObject& gameObject, sf::RenderWindow& renderWindow, sf::View view);
        void autoZoomCamera(sf::Vector2f p0, sf::Vector2f p1);

        bool init() override;
        void update(float deltaTime) override;
        void draw() override;

        sf::View getView()
        {
            return m_view;
        }

    private:
        sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);
        sf::View m_view;
        float m_minZoom = 500.0f;
    };
}
