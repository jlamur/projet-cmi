#include "player.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>
#include <iostream>

const unsigned int Player::TYPE_ID = 1;

Player::Player() : Object() {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
}

Player::~Player() {}

std::shared_ptr<Object> Player::load(std::ifstream& file) {
    std::shared_ptr<Object> object = std::shared_ptr<Object>(new Player);
    std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(object);

    // lecture du numéro de joueur
    char player_number;
    file.read(&player_number, 1);
    player->setPlayerNumber(player_number);

    // lecture des propriétés communes des objets
    Object::load(file, object);
    file.seekg(1, file.cur);

    return object;
}

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
    sprite.rotate(getVelocity().x * Constants::PHYSICS_TIME.asSeconds() * .5f);
    sprite.setPosition(getPosition());
    manager.getWindow().draw(sprite);
}

std::unique_ptr<sf::FloatRect> Player::getAABB() const {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    ));
}

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
