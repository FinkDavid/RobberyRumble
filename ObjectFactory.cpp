/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "CollideableManager.h"
#include "ColliderComponent.hpp"
#include "GameObjectEvents.hpp"
#include "HumanControllerComponent.h"
#include "ItemInfoComponent.h"
#include "ObjectFactory.hpp"
#include "PlayerAnimationComponent.h"
#include "PlayerItemDisplayComponent.h"
#include "PlayerMoveComponent.hpp"
#include "PlayerPickUpComponent.h"
#include "PlayerProgressBar.h"
#include "SpriteRenderComponent.hpp"
#include "Tileson.hpp"


namespace mmt_gd
{
    static GameObject::Ptr loadSprite(tson::Object& object,
                                      const std::string& layer,
                                      const fs::path& resourcePath,
                                      const SpriteManager& spriteManager,
                                      std::vector<tson::Object> dynamicColliders)
    {
        auto gameObject = GameObject::create(object.getName());

        gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));

        // Parse data from file
        sf::IntRect textureRect{};
        textureRect.width = object.getSize().x;
        textureRect.height = object.getSize().y;
        fs::path spriteTexture;
        fs::path bulletTexture;
        auto input = false;
        auto playerIdx = 0;
        bool isSensor = false;
        bool itemInfo = false;
        int itemValue;
        int itemWeight;
        int textPosX;
        int textPosY;
        std::string vanId;

        for (const auto* property : object.getProperties().get())
        {
            if (auto name = property->getName(); name == "Texture")
            {
                spriteTexture = resourcePath / std::any_cast<std::string>(property->getValue());
            }
            else if (name == "TextureRectLeft")
            {
                textureRect.left = std::any_cast<int>(property->getValue());
            }
            else if (name == "TextureRectTop")
            {
                textureRect.top = std::any_cast<int>(property->getValue());
            }
            else if (name == "InputPlayerIdx")
            {
                input = true;
                playerIdx = std::any_cast<int>(property->getValue());
            }
            else if (name == "IsSensor")
            {
                isSensor = std::any_cast<bool>(property->getValue());
            }
            else if (name == "Value")
            {
                itemValue = std::any_cast<int>(property->getValue());
                itemInfo = true;
            }
            else if (name == "Weight")
            {
                itemWeight = std::any_cast<int>(property->getValue());
                itemInfo = true;
            }
            else if (name == "textPosX")
            {
                textPosX = std::any_cast<int>(property->getValue());
                itemInfo = true;
            }
            else if (name == "textPosY")
            {
                textPosY = std::any_cast<int>(property->getValue());
                itemInfo = true;
            }
            else if (name == "VanId")
            {
                vanId = std::any_cast<std::string>(property->getValue());
            }
            else if (name == "LinkedCollider")
            {
                for (auto& collider : dynamicColliders)
                {
                    if (collider.getName() == std::any_cast<std::string>(property->getValue()))
                    {
                        sf::FloatRect colliderRect{};
                        colliderRect.width = collider.getSize().x;
                        colliderRect.height = collider.getSize().y;
                        colliderRect.top = static_cast<float>(collider.getPosition().y);
                        colliderRect.left = static_cast<float>(collider.getPosition().x);

                        auto collider = gameObject->addComponent<ColliderComponent>(*gameObject, colliderRect);

                        CollideableManager::getInstance().addCollider(collider);

                        sf::FloatRect hitBoxRect = colliderRect;

                        hitBoxRect.top -= 15;
                        hitBoxRect.left += 15;
                        hitBoxRect.height += 30;
                        hitBoxRect.width += 30;

                        auto hitBoxCollider = gameObject->addComponent<ColliderComponent>(*gameObject, hitBoxRect);
                        hitBoxCollider->setTrigger(true);
                        CollideableManager::getInstance().addCollider(hitBoxCollider);
                    }
                }
            }
        }

        if (spriteTexture.string().length() > 0)
        {
            const auto renderComp = gameObject->addComponent<SpriteRenderComponent>(*gameObject,
                                                                                    spriteManager.getWindow(),
                                                                                    spriteTexture.string(),
                                                                                    layer);

            renderComp->getSprite().setTextureRect(textureRect);
        }

        if (itemInfo)
        {
            AssetManager::getInstance().loadTexture(gameObject->getId(), "../assets/ItemIcons/" + gameObject->getId() + ".png");
            std::cout << "../assets/ItemIcons/" + gameObject->getId() + ".png" << std::endl;
            gameObject->addComponent<ItemInfoComponent>(*gameObject, itemValue, itemWeight, sf::Vector2f(textPosX, textPosY));
        }

        if (input)
        {
            gameObject->setTag("Player");

            IController::Ptr controller = gameObject->addComponent<HumanControllerComponent>(*gameObject, playerIdx);
            {
                controller = gameObject->addComponent<HumanControllerComponent>(*gameObject, playerIdx);
            }

            gameObject->addComponent<PlayerMoveComponent>(*gameObject, controller, playerIdx);

            gameObject->addComponent<PlayerAnimationComponent>(*gameObject, textureRect);

            gameObject->addComponent<PlayerPickUpComponent>(*gameObject, playerIdx, vanId);

            gameObject->addComponent<PlayerProgressBar>(*gameObject, playerIdx);

            gameObject->addComponent<PlayerItemDisplayComponent>(*gameObject, playerIdx);
        }

        if (!gameObject->init())
        {
            sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap " << std::endl;
        }

        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

        return gameObject;
    }

    static GameObject::Ptr loadCollider(const tson::Object& object, const std::string& layer)
    {
        auto gameObject = GameObject::create(object.getName());
        gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));
        gameObject->setTag(object.getType());

        auto collider = gameObject
                            ->addComponent<ColliderComponent>(*gameObject,
                                                              sf::FloatRect(static_cast<float>(object.getPosition().x),
                                                                            static_cast<float>(object.getPosition().y),
                                                                            object.getSize().x,
                                                                            object.getSize().y));

        CollideableManager::getInstance().addCollider(collider);

        if (!gameObject->init())
        {
            sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap " << std::endl;
        }

        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

        return gameObject;
    }

    static GameObject::Ptr loadTrigger(tson::Object& object, const std::string& layer)
    {
        auto tag = object.getType();
        auto gameObject = GameObject::create(object.getName());
        gameObject->setPosition(static_cast<float>(object.getPosition().x), static_cast<float>(object.getPosition().y));
        gameObject->setTag(tag);

        auto collider = gameObject
                            ->addComponent<ColliderComponent>(*gameObject,
                                                              sf::FloatRect(static_cast<float>(object.getPosition().x),
                                                                            static_cast<float>(object.getPosition().y),
                                                                            object.getSize().x,
                                                                            object.getSize().y));
        collider->setTrigger(true);

        CollideableManager::getInstance().addCollider(collider);


        if (!gameObject->init())
        {
            sf::err() << "Could not initialize go " << gameObject->getId() << " in TileMap " << std::endl;
        }

        EventBus::getInstance().fireEvent(std::make_shared<GameObjectCreateEvent>(gameObject));

        for (const auto* property : object.getProperties().get())
        {
            if (auto name = property->getName(); name == "LinkedTrigger")
            {
                collider->setLinkedTrigger(std::any_cast<std::string>(property->getValue()));
            }
        }

        return gameObject;
    }

    GameObject::Ptr ObjectFactory::processTsonObject(
        tson::Object& object,
        const tson::Layer& layer,
        const fs::path& path,
        const SpriteManager& spriteManager,
        std::vector<tson::Object> dynamicColliders)
    {
        if (object.getType() == "Sprite")
        {
            auto sprite = loadSprite(object, layer.getName(), path, spriteManager, dynamicColliders);
        }
        if (object.getType() == "Collider")
        {
            auto collider = loadCollider(object, layer.getName());
        }
        if (object.getType() == "Trigger" || object.getType() == "DepositTrigger")
        {
            auto trigger = loadTrigger(object, layer.getName());
        }

        return {};
    }
}
