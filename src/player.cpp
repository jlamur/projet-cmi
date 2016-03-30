#include "player.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>
#include <iostream>

Player::Player(float x, float y) : Object(x, y) {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
}

Player::~Player() {}

sf::Vector2f Player::getForces(const Manager& manager, const std::vector<ObjectPtr>& objects) const {
    sf::Vector2f forces = Object::getForces(manager, objects);

    // déplacement de la balle après appui sur les touches de direction
    if (getPlayerNumber() == 0) {
        if (manager.isKeyPressed(sf::Keyboard::Left)) {
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if (manager.isKeyPressed(sf::Keyboard::Right)) {
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }

    if (getPlayerNumber() == 1) {
        if (manager.isKeyPressed(sf::Keyboard::Q)) {
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if (manager.isKeyPressed(sf::Keyboard::D)) {
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }

    return forces;
}

void Player::draw(Manager& manager) {
    Object::draw(manager);

    // utilisation de la texture
	sprite.setTexture(
        manager.getResourceManager().getTexture("ball.png")
    );

    // déplacement du sprite à la position de la balle
    sprite.rotate(getVelocity().x * Constants::PHYSICS_TIME * .5f);
    sprite.setPosition(getPosition());
    manager.getWindow().draw(sprite);
}

std::unique_ptr<sf::FloatRect> Player::getAABB() const {
    return std::make_unique<sf::FloatRect>(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    );
}

const unsigned int Player::TYPE_ID = 0;
unsigned int Player::getTypeId() const {
    return TYPE_ID;
}

float Player::getRadius() const {
    return 10 * getMass();
}

unsigned int Player::getPlayerNumber() const {
    return player_number;
}

void Player::setPlayerNumber(unsigned int set_number) {
    player_number = set_number;
}
