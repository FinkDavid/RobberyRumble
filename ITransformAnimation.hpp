/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once
namespace mmt_gd
{
class ITransformAnimation
{
public:
    using Ptr = std::shared_ptr<ITransformAnimation>;

    virtual ~ITransformAnimation() = default;

    virtual void update(float deltaTime, sf::Transformable& transform) = 0;
};
} // namespace mmt_gd
