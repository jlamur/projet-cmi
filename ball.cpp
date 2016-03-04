#include "ball.hpp"
#include "engine.hpp"

// TODO réécrire cette classe

Ball::Ball() {
    if (!texture.loadFromFile("res/balle.png")) {
        std::cerr << "Failed to load texture" << std::endl;
    }

    velocity.x = velocity.y = 0;
    position.x = 150; position.y = 150;

    sprite.setOrigin(12, 12);
    sprite.setTexture(texture);
}

void Ball::update(float delta) {
    int verticalPos = std::ceil(position.y);

    // gravity
    velocity.y += Engine::GRAVITY * delta;

    // jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && verticalPos >= 150) {
        velocity.y = -Engine::JUMP;
    }

    // go left/right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x -= Engine::MOVE;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x += Engine::MOVE;
    }

    if (verticalPos > 150) {
        position.y = 150;
        velocity.y = -.3f * std::abs(velocity.y);
    }

    velocity.x *= .95f;
    position += velocity * delta;
}

void Ball::draw(sf::RenderWindow& window) {
    sprite.setPosition(position + sf::Vector2f(0, -12));
    sprite.rotate(velocity.x / 10);

    window.draw(sprite);
}
