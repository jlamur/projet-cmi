#include "player.hpp"
#include "level.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>

const unsigned int Player::TYPE_ID = 1;

Player::Player() : Object() {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
    sprite.setRadius(getRadius());
    sprite.setOutlineColor(sf::Color::Black);
    sprite.setOutlineThickness(1.5f);
}

Player::~Player() {}

ObjectPtr Player::clone() const {
    return ObjectPtr(new Player(*this));
}

ObjectPtr Player::load(std::ifstream& file) {
    ObjectPtr object = ObjectPtr(new Player);
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

sf::Vector2f Player::getForces(const Level& level) const {
    sf::Vector2f forces = Object::getForces(level);

    // déplacement de la balle après appui sur les touches de direction
    if (getPlayerNumber() == 0) {
        if (level.isKeyPressed(sf::Keyboard::Left)) {
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if (level.isKeyPressed(sf::Keyboard::Right)) {
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }

    if (getPlayerNumber() == 1) {
        if (level.isKeyPressed(sf::Keyboard::Q)) {
            forces += sf::Vector2f(-Constants::MOVE, 0);
        }

        if (level.isKeyPressed(sf::Keyboard::D)) {
            forces += sf::Vector2f(Constants::MOVE, 0);
        }
    }

    return forces;
}

void Player::draw(Level& level) {
    // utilisation de la texture
	sprite.setTexture(
        &level.getResourceManager().getTexture("player.tga")
    );

    // si le joueur est sélectionné, on met sa bordure en rouge
    // if (isSelected()) {
    //     sprite.setColor(sf::Color(255, 0, 0));
    // } else {
        // coloration du joueur en fonction de son numéro
        if (getPlayerNumber() == 0) {
            sprite.setFillColor(sf::Color(239, 83, 80));
        } else if (getPlayerNumber() == 1) {
            sprite.setFillColor(sf::Color(92, 107, 192));
        }
    // }

    // déplacement du sprite à la position de la balle
    sprite.setPosition(getPosition());
    level.getWindow().draw(sprite);
}

void Player::activated(Level& level, Object& object) {
    // ne rien faire si le joueur est activé.
    // en règle générale, c'est l'objet activé par le joueur
    // qui s'occupe de la réponse
}

void Player::updatePosition() {
    // calcul de la différence de position pour connaître
    // (approximativement) la rotation de la balle
    sf::Vector2f last_position = getPosition();
    Object::updatePosition();
    sprite.rotate((getPosition() - last_position).x * 3.f);
}

std::unique_ptr<sf::FloatRect> Player::getAABB() const {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    ));
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
