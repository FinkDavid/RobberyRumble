/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#pragma once
#include "GameObject.hpp"
#include "SpriteManager.hpp"
#include "Tileson.hpp"

namespace tson
{
    class Object;
}

namespace mmt_gd
{
    class ObjectFactory
    {
    public:
        static GameObject::Ptr processTsonObject(tson::Object&        object,
                                                 const tson::Layer&   layer,
                                                 const fs::path&      path,
                                                 const SpriteManager& spriteManager,
                                                 std::vector<tson::Object> dynamicColliders);
    };
}
