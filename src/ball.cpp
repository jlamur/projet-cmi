#include "ball.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>
#include <iostream>

Ball::Ball(float x, float y) : Object(x, y) {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
}

sf::Vector2f Ball::getForces(EngineState& state) {
    sf::Vector2f forces = Object::getForces(state);

    // déplacement de la balle après appui sur les touches de direction
    if (state.keys[sf::Keyboard::Left]) {
        forces += sf::Vector2f(-Constants::MOVE, 0);
    }

    if (state.keys[sf::Keyboard::Right]) {
        forces += sf::Vector2f(Constants::MOVE, 0);
    }

    return forces;
}

void Ball::draw(sf::RenderWindow& window, ResourceManager& resources) {
    Object::draw(window, resources);

    // utilisation de la texture
	sprite.setTexture(resources.getTexture("ball.png"));

    // déplacement du sprite à la position de la balle
    sprite.rotate(getVelocity().x * .1f);
    sprite.setPosition(getPosition());
    window.draw(sprite);
}

std::unique_ptr<sf::FloatRect> Ball::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    ));
}

unsigned int Ball::getTypeId() {
    return Ball::TYPE_ID;
}

float Ball::getRadius() {
    return 10 * getMass();
}
