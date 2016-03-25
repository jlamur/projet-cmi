#include "player.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>
#include <iostream>

Player::Player(float x, float y) : Object(x, y) {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
}

sf::Vector2f Player::getForces(EngineState& state) {
    sf::Vector2f forces = Object::getForces(state);

    //commandes du joueur 1
    if(player_number==1){
        // déplacement de la balle après appui sur les touches de direction
        if (state.keys[sf::Keyboard::Left]) {
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if (state.keys[sf::Keyboard::Right]) {
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }
    else{
        if(state.keys[sf::Keyboard::Q]){
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if(state.keys[sf::Keyboard::D]){
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }

    return forces;
}

void Player::draw(sf::RenderWindow& window, ResourceManager& resources) {
    Object::draw(window, resources);

    // utilisation de la texture
	sprite.setTexture(resources.getTexture("ball.bmp"));

    // déplacement du sprite à la position de la balle
    sprite.rotate(getVelocity().x * .1f);
    sprite.setPosition(getPosition());
    window.draw(sprite);
}

std::unique_ptr<sf::FloatRect> Player::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    ));
}

unsigned int Player::getTypeId() {
    return Player::TYPE_ID;
}

float Player::getRadius() {
    return 10 * getMass();
}

unsigned int Player::getPlayerNumber() {
    return player_number;
}

void Player::setPlayerNumber(unsigned int set_number) {
    player_number = set_number;
}
