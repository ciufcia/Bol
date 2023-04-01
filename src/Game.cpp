#include "Game.hpp"

#include "Renderable.hpp"
#include "Renderer.hpp"

#include "Rigidbody.hpp"
#include "RigidbodyPhysics.hpp"

#include "Player.hpp"
#include "PlayerController.hpp"

#include "Bouncer.hpp"
#include "BouncerController.hpp"

#include "CircleCollider.hpp"

#include "BouncerManager.hpp"

#include "ScoreCounter.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <secs.hpp>

#include <iostream>

const float WINDOW_WIDTH = 350.f;
const float HALF_WINDOW_WIDTH = WINDOW_WIDTH / 2.f;

const float WINDOW_HEIGHT = 600.f;
const float HALF_WINDOW_HEIGHT = WINDOW_HEIGHT / 2.f;

void bol::Game::setup() {
    ecs.registerComponent<bol::Renderable>();
    ecs.registerComponent<bol::Rigidbody>();
    ecs.registerComponent<bol::Player>();
    ecs.registerComponent<bol::CircleCollider>();
    ecs.registerComponent<bol::Bouncer>();

    secs::Entity player = ecs.createEntity();

    ecs.activateSystem<bol::Renderer>(&renderer);
    ecs.setSystemDependency<bol::Renderer, bol::Renderable>(true);

    ecs.activateSystem<bol::RigidbodyPhysics>(&rigidbodyPhysics);
    ecs.setSystemDependency<bol::RigidbodyPhysics, bol::Rigidbody>(true);

    ecs.activateSystem<bol::PlayerController>(&playerController);
    ecs.setSystemDependency<bol::PlayerController, bol::Player>(true);
    ecs.setSystemDependency<bol::PlayerController, bol::Rigidbody>(true);

    sf::Texture t;
    t.loadFromFile("assets/bouncer-activated.png");
    bouncerController.activeTexture = t;
    t.loadFromFile("assets/bouncer-idle.png");
    bouncerController.idleTexture = t;

    ecs.activateSystem<bol::BouncerController>(&bouncerController);
    ecs.setSystemDependency<bol::BouncerController, bol::Bouncer>(true);
    ecs.setSystemDependency<bol::BouncerController, bol::Renderable>(true);
}

void bol::Game::mainLoop() {
    sf::Image icon;
    icon.loadFromFile("assets/bol.png");

    sf::Clock clock;

    sf::RenderWindow window {sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bol", sf::Style::Close | sf::Style::Titlebar};

    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Event event;

    window.setFramerateLimit(60u);

    sf::Sprite title {};

    sf::Texture titleTexture;
    titleTexture.loadFromFile("assets/title.png");
    sf::Sprite titleFull {titleTexture};
    titleFull.setOrigin({26.f, 8.f});
    titleFull.setPosition({HALF_WINDOW_WIDTH, 290.f});
    titleFull.setScale({4.f, 4.f});

    sf::Texture titleEmptyTexture;
    titleEmptyTexture.loadFromFile("assets/title-empty.png");
    sf::Sprite titleEmpty {titleEmptyTexture};
    titleEmpty.setOrigin({26.f, 8.f});
    titleEmpty.setPosition({HALF_WINDOW_WIDTH, 290.f});
    titleEmpty.setScale({4.f, 4.f});

    sf::Font gameFont;
    gameFont.loadFromFile("assets/font.ttf");

    sf::Text titleText;
    titleText.setFont(gameFont);
    titleText.setFillColor({0u, 0u, 0u});
    titleText.setString("Press any key to play!");
    titleText.setCharacterSize(21u);
    titleText.setOrigin({titleText.getGlobalBounds().getSize().x / 2.f, titleText.getGlobalBounds().getSize().y / 2.f});
    titleText.setPosition({HALF_WINDOW_WIDTH, 350.f});

    secs::Entity player = ecs.createEntity();

    bol::Renderable renderable;
    renderable.load("assets/bol.png");
    renderable.sprite.setOrigin({8.f, 8.f});
    renderable.sprite.setScale({4.f, 4.f});
    ecs.addComponent<bol::Renderable>(player, renderable);

    ecs.addComponent<bol::Rigidbody>(player);

    ecs.addComponent<bol::Player>(player);

    ecs.addComponent<bol::CircleCollider>(player);

    sf::SoundBuffer deathSoundBuffer {};
    deathSoundBuffer.loadFromFile("assets/death.wav");
    sf::Sound deathSound {};
    deathSound.setBuffer(deathSoundBuffer);

    while (window.isOpen()) {

        sf::View view {sf::Vector2f(HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT), sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)};

        window.setView(view);

        title = titleFull;
        titleText.setRotation(0.f);
        float textRotationDirection = 1.f;

        title.setColor(sf::Color(255u, 255u, 255u, 255u));
        titleText.setFillColor(sf::Color(0u, 0u, 0u, 255u));

        while (window.isOpen()) {

            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        return;
                    case sf::Event::KeyPressed:
                        title = titleEmpty;
                        goto exitMenu;
                    default:
                        break;
                }
            }

            float frameTime = clock.restart().asSeconds();

            titleText.rotate(textRotationDirection * 10.f * frameTime);

            if (textRotationDirection > 0.f && titleText.getRotation() > 10.f && titleText.getRotation() < 350.f) {
                titleText.setRotation(10.f);
                textRotationDirection = -1.f;
            } else if (textRotationDirection < 0.f && titleText.getRotation() < 350.f && titleText.getRotation() > 10.f) {
                titleText.setRotation(350.f);
                textRotationDirection = 1.f;
            }

            window.clear({255u, 255u, 255u});

            window.draw(title);
            window.draw(titleText);

            window.display();
        }

        exitMenu: ;

        bol::BouncerManager bouncerManager {&ecs};

        bouncerManager.setup();

        bol::ScoreCounter scoreCounter {};

        float fixedUpdateTimeAccumulator = 0.f;

        scoreCounter.setScale({10.f, 10.f});
        scoreCounter.setCenter(view.getCenter());
        scoreCounter.setOpacity(155u);

        float cameraMoveSpeed = 25.f;
        const float cameraAccelerationRate = 1.f;

        bol::Rigidbody *playerRigidBody = ecs.getComponent<bol::Rigidbody>(player);
        playerRigidBody->position = {HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT};
        playerRigidBody->velocity = {0.f, 0.f};
        playerRigidBody->force = {0.f, 0.f};

        bol::Renderable *playerRenderable = ecs.getComponent<bol::Renderable>(player);
        playerRenderable->sprite.setPosition({HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT});
        playerRenderable->sprite.setRotation(0.f);

        bol::CircleCollider *playerCollider = ecs.getComponent<bol::CircleCollider>(player);
        playerCollider->center = {HALF_WINDOW_WIDTH, HALF_WINDOW_HEIGHT};
        playerCollider->radius = 32.f;

        float titleOpacity = 255.f;

        clock.restart();

        while (window.isOpen()) {

            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        return;
                    default:
                        break;
                }
            }

            float frameTime = clock.restart().asSeconds();

            fixedUpdateTimeAccumulator += frameTime;

            while (fixedUpdateTimeAccumulator >= 0.016f) {
                rigidbodyPhysics.update();
                playerController.fixedUpdate(350.f);

                fixedUpdateTimeAccumulator -= 0.016f;
            }

            bouncerManager.update(view);
            playerController.update(frameTime);
            bouncerController.update(frameTime);

            scoreCounter.incrementScore(frameTime);

            view.move({0.f, -cameraMoveSpeed * frameTime});
            scoreCounter.move({0.f, -cameraMoveSpeed * frameTime});

            float scaleFactor = 300.f / (scoreCounter.getDigitTextureSize().x * scoreCounter.getAmountOfDigits());

            scoreCounter.setScale({scaleFactor, scaleFactor});

            float newSpeed = cameraMoveSpeed + cameraAccelerationRate * frameTime;

            if (newSpeed > 160.f)
                cameraMoveSpeed = 160.f;
            else
                cameraMoveSpeed = newSpeed;

            titleOpacity -= 100.f * frameTime;

            if (titleOpacity < 0.f)
                titleOpacity = 0.f;

            title.setColor(sf::Color(255u, 255u, 255u, titleOpacity));
            titleText.setFillColor(sf::Color(0u, 0u, 0u, titleOpacity));

            window.setView(view);

            window.clear(sf::Color(255u, 255u, 255u));

            scoreCounter.draw(window);

            window.draw(title);
            window.draw(titleText);

            renderer.render(window);

            window.display();

            if (ecs.getComponent<bol::Rigidbody>(player)->position.y - 32.f > view.getCenter().y + HALF_WINDOW_HEIGHT) {
                deathSound.play();
                break;
            }
        }

        playerRenderable = ecs.getComponent<bol::Renderable>(player);
        playerRenderable->sprite.setPosition(playerRenderable->sprite.getPosition().x, view.getCenter().y + HALF_WINDOW_HEIGHT + 32.f);
        playerRenderable->sprite.setScale({4.f, 4.f});

        float animationXVelocity = HALF_WINDOW_WIDTH - playerRenderable->sprite.getPosition().x;

        sf::Vector2f animationVelocity {animationXVelocity, -800.f};

        float rotationDirection = 0.f;

        if (animationXVelocity != 0.f)
            rotationDirection = animationXVelocity / abs(animationXVelocity);

        float rotationVelocity = 100.f;

        clock.restart();

        while (window.isOpen()) {

            while (window.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        return;
                    default:
                        break;
                }
            }

            float frameTime = clock.restart().asSeconds();

            playerRenderable->sprite.move(animationVelocity * frameTime);
            playerRenderable->sprite.rotate(rotationVelocity * frameTime * rotationDirection);
            animationVelocity.y += 1000.f * frameTime;
            rotationVelocity += 200.f * frameTime;

            window.clear(sf::Color(255u, 255u, 255u));

            scoreCounter.draw(window);

            window.draw(title);
            window.draw(titleText);

            renderer.render(window);
        
            window.display();

            if (animationVelocity.y > 0.f && playerRenderable->sprite.getPosition().y > view.getCenter().y + HALF_WINDOW_HEIGHT + 32.f)
                break;
        }

    }
}