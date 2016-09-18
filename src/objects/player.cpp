#include "utility.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/player.hpp"

const unsigned int Player::TYPE_ID = 1;

LevelData::ObjectPtr Player::load(std::ifstream& file) {
    auto object = std::shared_ptr<Player>(new Player);
    object->read(file);
    return object;
}

Player::Player(sf::Vector2f position) : Object(position) {
    sprite.setOutlineThickness(1.5f);
    sprite.setRadius(10);
    sprite.setOrigin(sf::Vector2f(10, 10));

    shadow_sprite.setRadius(9);
    shadow_sprite.setOrigin(sf::Vector2f(9, 9));
}

LevelData::ObjectPtr Player::clone() const {
    return LevelData::ObjectPtr(new Player(*this));
}

void Player::draw(sf::RenderWindow& window) {
    // on charge la texture selon le numéro du joueur
    std::string texture_name;

    if (getPlayerNumber() == 0) {
        texture_name = "player_1.tga";
    } else {
        texture_name = "player_2.tga";
    }

    sprite.setTexture(ResourceManager::get().getTexture("objects/" + texture_name).get());
    sprite.setPosition(getPosition());

    shadow_sprite.setTexture(ResourceManager::get().getTexture("objects/player_shadow.tga").get());
    shadow_sprite.setPosition(getPosition());

    // on fait tourner le sprite selon la différence de position
    // if (previous_position != sf::Vector2f(0, 0)) {
    //     sf::Vector2f right_dir = level.getRightDirection();
    //     sf::Vector2f diff = getPosition() - previous_position;
    //
    //     float rotate = (right_dir.x * diff.x + right_dir.y * diff.y) * 3 / 200;
    //     sprite.rotate(rotate);
    // }
    //
    // previous_position = getPosition();

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
        sprite.setFillColor(sf::Color::White);
    }

    // dessin des sprites
    window.draw(sprite);
    window.draw(shadow_sprite);
}

unsigned int Player::getTypeId() const {
    return Player::TYPE_ID;
}

Geometry Player::getGeometry() const {
    return Geometry(getPosition(), 10);
}

unsigned int Player::getPlayerNumber() const {
    return player_number;
}

void Player::setPlayerNumber(unsigned int player_number) {
    this->player_number = player_number;
}

void Player::activate(Game& game, Object& object) {
    // aucune action en cas d'activation du joueur
    // en règle générale, c'est l'objet activé par le joueur qui y répond
}

sf::Vector2f Player::getForces(const Game& game) const {
    sf::Vector2f forces = Object::getForces(game);

    // déplacement de la balle si appui sur les touches de direction
    if (game.getManager().getWindow().hasFocus()) {
        if (getPlayerNumber() == 0) {
            if (Utility::isKeyPressed(sf::Keyboard::Left)) {
                forces += game.getLeftDirection();
            }

            if (Utility::isKeyPressed(sf::Keyboard::Right)) {
                forces += game.getRightDirection();
            }
        }

        if (getPlayerNumber() == 1) {
            if (Utility::isKeyPressed(sf::Keyboard::Q)) {
                forces += game.getLeftDirection();
            }

            if (Utility::isKeyPressed(sf::Keyboard::D)) {
                forces += game.getRightDirection();
            }
        }
    }

    return forces;
}
