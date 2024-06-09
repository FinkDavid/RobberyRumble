/*
MultiMediaTechnology / FHS
MultiMediaProjekt 2
Developed by Dominik Farmer and David Fink
*/

#include "stdafx.h"

#include "CameraRenderComponent.hpp"
#include "CollideableManager.h"
#include "ColliderComponent.hpp"
#include "Game.hpp"
#include "GameObjectManager.hpp"
#include "InputManager.hpp"
#include "MainState.hpp"
#include "PlayerItemDisplayComponent.h"
#include "PlayerMoveComponent.hpp"
#include "PlayerPickUpComponent.h"
#include "PlayerProgressBar.h"
#include "TileMapLoader.hpp"
#include "Tileson.hpp"
#include "TransformAnimationComponent.hpp"
#include "TransformAnimationSmoothFollow.hpp"

#include <memory>
#include <thread>

namespace mmt_gd
{
    MainState::MainState(GameStateManager* gameStateManager, Game* game) :
    GameState(gameStateManager, game),
    m_spriteManager(game->getWindow())
    {
    }

    void MainState::init()
    {
        PROFILE_FUNCTION();

        GameObjectManager::getInstance().init();
        m_spriteManager.init();

        // load tile map/level
        {
            PROFILE_SCOPE("Load Tile Map");
            const auto resourcePath = fs::path{"../"} / "assets";
            const auto mapPath = resourcePath / "Map.tmj";

            tson::Tileson t;
            if (const std::unique_ptr<tson::Map> map = t.parse(mapPath); map->getStatus() == tson::ParseStatus::OK)
            {
                TileMapLoader::loadTileLayers(map, resourcePath, m_spriteManager);
                TileMapLoader::loadObjectLayers(map, resourcePath, m_spriteManager);
            }
            else
            {
                std::cerr << "Error loading " << mapPath << std::endl;
            }
        }


        m_cameraTarget = GameObject::create("cameraTarget");
        m_camera = GameObject::create("Camera");
        // Moving camera
        {
            const auto renderComp = m_camera->addComponent<CameraRenderComponent>(*m_camera,
                                                                                  m_game->getWindow(),
                                                                                  m_game->getWindow().getView());

            m_camera->addComponent<
                TransformAnimationComponent>(*m_camera,
                                             std::make_shared<mmt::TransformAnimationSmoothFollow>(m_cameraTarget, 10.F));

            if (!m_camera->init())
                ffErrorMsg("Could not initialize camera");

            GameObjectManager::getInstance().addGameObject(m_camera);
            m_spriteManager.setCamera(renderComp.get());
        }


        auto& objects = GameObjectManager::getInstance().getGameObjects();

        for (auto& object : objects)
        {
            if (object.second->getTag() == "Trigger")
            {
                if (auto triggerComponent = object.second->getComponent<ColliderComponent>())
                {
                    triggerComponent->registerOnTriggerFunction(
                        [this](ColliderComponent& a, ColliderComponent& b)
                        {
                            std::cout << " Trigger: " << a.getGameObject().getId()
                                      << " | GO: " << b.getGameObject().getId() << std::endl;
                            auto& pickUp = GameObjectManager::getInstance().getGameObject(a.getLinkedTriggerId());

                            auto& player = b.getGameObject();

                            auto& trigger = GameObjectManager::getInstance().getGameObject(a.getGameObject().getId());
                            player.getComponent<PlayerPickUpComponent>()->addPickUp(pickUp, trigger);
                            auto& itemDisplayComponent = player.getComponent<PlayerItemDisplayComponent>();
                            itemDisplayComponent->updateIcon(AssetManager::getInstance().getTexture(pickUp->getId()));
                            itemDisplayComponent->showIcon(true);
                        });
                }
            }
            else if (object.second->getTag() == "DepositTrigger")
            {
                if (auto triggerComponent = object.second->getComponent<ColliderComponent>())
                {
                    triggerComponent->registerOnTriggerFunction(
                        [](ColliderComponent& a, ColliderComponent& b)
                        {
                            std::cout << " Trigger: " << a.getGameObject().getId()
                                      << " | GO: " << b.getGameObject().getId() << std::endl;

                            auto& player = b.getGameObject();
                            player.getComponent<PlayerPickUpComponent>()->depositPickUp();
                            player.getComponent<PlayerItemDisplayComponent>()->showIcon(false);
                        });
                }
            }
            else if (object.second->getTag() == "Collider")
            {
                if (auto colliderComponent = object.second->getComponent<ColliderComponent>())
                {
                    colliderComponent->registerOnCollisionFunction(
                        [](ColliderComponent& a, ColliderComponent& b) {
                            std::cout << " Collider: " << a.getGameObject().getId()
                                      << " | GO: " << b.getGameObject().getId() << std::endl;
                        });
                }
            }
        }
        m_originalWindowSize = Game::getWindow().getSize().x;
        m_timerText = tgui::Label::create();
        m_timerText->setOrigin(0.5, 0);
        m_timerText->setPosition((m_originalWindowSize / 2), 10.0f * (m_originalWindowSize / m_madeWindowSize));
        m_timerText->setTextSize(m_timerFontSize * (m_originalWindowSize / m_madeWindowSize));
        Game::getGui().add(m_timerText);

        m_spriteManager.setLayerOrder(
            {"ObjectsBehindPlayer",
             "GameObjectsBehindPlayer",
             "ObjectsBehindPlayer2",
             "Player",
             "ObjectsInFrontOfPlayer",
             "GameObjectsInFrontOfPlayer"});
    }

    void MainState::update(float deltaTime)
    {
        PROFILE_FUNCTION();
        if (InputManager::getInstance().isKeyPressed("Exit"))
        {
            m_gameStateManager->setState("MenuState");
            return;
        }

        if (deltaTime >= 1.0f)
        {
            deltaTime = 0.0f;
        }

        float previousFrameFactor = m_resizeFactor;
        m_resizeFactor = Game::getWindow().getSize().x / m_originalWindowSize;

        if (m_resizeFactor != previousFrameFactor)
        {
            m_originalWindowSize = Game::getWindow().getSize().x;
            m_timerText->setTextSize(m_timerText->getTextSize() * m_resizeFactor);
            m_timerText->setPosition(m_timerText->getPosition().x * m_resizeFactor, m_timerText->getPosition().y * m_resizeFactor);
        }

        if (!m_roundStarted)
        {
            m_preRoundTimer += deltaTime;
            if ((int)(m_preRoundTime - m_preRoundTimer) == 0)
            {
                m_timerText->setText("STEAL!!");
            }
            else
            {
                m_timerText->setText(tgui::String((int)(m_preRoundTime - m_preRoundTimer)));
            }

            if (m_preRoundTimer >= m_preRoundTime)
            {
                m_roundStarted = true;
                GameObjectManager::getInstance().getGameObject("Player0")->getComponent<PlayerMoveComponent>()->setCanMove(
                    true);
                GameObjectManager::getInstance().getGameObject("Player1")->getComponent<PlayerMoveComponent>()->setCanMove(
                    true);
            }
        }
        else
        {
            m_roundTimer += deltaTime;

            m_timerText->setText(tgui::String((int)(m_roundTime - m_roundTimer)));
        }

        if (m_roundTimer >= m_roundTime)
        {
            m_gameStateManager->setState("FinishState");
        }

        EventBus::getInstance().processEvents(deltaTime);
        GameObjectManager::getInstance().update(deltaTime);
        auto p0 = GameObjectManager::getInstance().getGameObject("Player0");
        auto p1 = GameObjectManager::getInstance().getGameObject("Player1");
        m_cameraTarget->setPosition((p0->getPosition() + p1->getPosition()) / 2.0f);

        m_camera->getComponent<CameraRenderComponent>()->autoZoomCamera(p0->getPosition(), p1->getPosition());
        GameObjectManager::getInstance().getGameObject("Player0")->getComponent<PlayerProgressBar>()->setView(m_game->getWindow().getView());
    }

    void MainState::draw()
    {
        PROFILE_FUNCTION();
        m_spriteManager.draw();
    }

    void MainState::exit()
    {
        PROFILE_FUNCTION();
        m_spriteManager.shutdown();
        CollideableManager::getInstance().shutdown();
        m_roundTimer = 0.0f;
        m_preRoundTimer = 0.0f;
        m_roundStarted = false;
        m_game->getGui().removeAllWidgets();
    }
}
