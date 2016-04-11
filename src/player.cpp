#include "player.hpp"
#include "game.hpp"
#include "game.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <iostream>
#include <array>

const unsigned int Player::TYPE_ID = 1;

Player::Player() : Object() {
    // déplacement de l'origine au centre de la balle
    sprite.setOutlineThickness(1.5f);
}

Player::~Player() {}

Object::Ptr Player::clone() const {
    return Object::Ptr(new Player(*this));
}

void Player::init(std::ifstream& file, Object::Ptr object) {
    Player::Ptr player = std::dynamic_pointer_cast<Player>(object);

    // lecture des propriétés communes des objets
    Object::init(file, object);
}

Object::Ptr Player::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new Player);
    Player::init(file, object);
    return object;
}

void Player::save(std::ofstream& file) const {
    // écriture des propriétés communes
    Object::save(file);
}

sf::Vector2f Player::getForces(const Game& game) const {
    sf::Vector2f forces = Object::getForces(game);

    // déplacement de la balle après appui sur les touches de direction
    if (getPlayerNumber() == 0) {
        if (game.getManager().isKeyPressed(sf::Keyboard::Left)) {
            forces += game.getLeftDirection();
        }

        if (game.getManager().isKeyPressed(sf::Keyboard::Right)) {
            forces += game.getRightDirection();
        }
    }

    if (getPlayerNumber() == 1) {
        if (game.getManager().isKeyPressed(sf::Keyboard::Q)) {
            forces += game.getLeftDirection();
        }

        if (game.getManager().isKeyPressed(sf::Keyboard::D)) {
            forces += game.getRightDirection();
        }
    }

    return forces;
}

void Player::draw(Level& level) {
    // on charge la texture selon le numéro du joueur
    std::string texture_name;

    if (getPlayerNumber() == 0) {
        texture_name = "player_1.tga";
    } else {
        texture_name = "player_2.tga";
    }

    sprite.setTexture(&level.getResourceManager().getTexture(texture_name));
    sprite.setRadius(getRadius());
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
    sprite.setPosition(getPosition());

    shadow_sprite.setTexture(&level.getResourceManager().getTexture("player_shadow.tga"));
    shadow_sprite.setRadius(getRadius() - 1);
    shadow_sprite.setOrigin(sf::Vector2f(getRadius() - 1, getRadius() - 1));
    shadow_sprite.setPosition(getPosition());

    // on fait tourner le sprite selon la différence de position
    if (previous_position != sf::Vector2f(0, 0)) {
        sf::Vector2f right_dir = level.getRightDirection();
        sf::Vector2f diff = getPosition() - previous_position;
        
        float rotate = (right_dir.x * diff.x + right_dir.y * diff.y) * 3 / 200;
        sprite.rotate(rotate);
    }

    previous_position = getPosition();

    // si le joueur est sélectionné, on met sa bordure en rouge
    if (isSelected()) {
        sprite.setOutlineColor(sf::Color(255, 0, 0));
    } else {
        sprite.setOutlineColor(sf::Color::Black);
    }

    // coloration du joueur en fonction de son numéro
    if (getCharge() > 0) {
        sprite.setFillColor(sf::Color(92, 107, 192));
    } else if (getCharge() < 0) {
        sprite.setFillColor(sf::Color(239, 83, 80));
    } else {
        sprite.setFillColor(sf::Color::Transparent);
    }

    // dessin des sprites
    level.getWindow().draw(sprite);
    level.getWindow().draw(shadow_sprite);
}

void Player::activate(Game& game, Object::Ptr object) {
    // ne rien faire si le joueur est activé.
    // en règle générale, c'est l'objet activé par le joueur
    // qui s'occupe de la réponse
}

sf::FloatRect Player::getAABB() const {
    return sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    );
}

float Player::getRadius() const {
    return 10 * getMass();
}

unsigned int Player::getTypeId() const {
    return TYPE_ID;
}

CollisionType Player::getCollisionType() const {
    return CollisionType::CIRCLE;
}

unsigned int Player::getPlayerNumber() const {
    return player_number;
}

void Player::setPlayerNumber(unsigned int set_number) {
    player_number = set_number;
}
