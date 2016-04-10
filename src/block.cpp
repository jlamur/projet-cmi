#include "block.hpp"
#include "level.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "resource_manager.hpp"

const unsigned int Block::TYPE_ID = 2;

Block::Block() : Object() {
    // par défaut, les blocs ne sont pas déplaçables et ont
    // donc une masse infinie, représentée par 0
    setMass(0.f);

    // déplacement de l'origine au centre du bloc
    sprite.setOrigin(sf::Vector2f(23, 23));
    select_sprite.setOrigin(sf::Vector2f(23, 23));
}

Block::~Block() {}

Object::Ptr Block::clone() const {
    return Object::Ptr(new Block(*this));
}

void Block::init(std::ifstream& file, Object::Ptr object) {
    // lecture des propriétés communes des objets
    Object::init(file, object);
}

Object::Ptr Block::load(std::ifstream& file) {
    Object::Ptr object = Object::Ptr(new Block);
    Block::init(file, object);
    return object;
}

void Block::save(std::ofstream& file) const {
    // écriture des propriétés communes
    Object::save(file);
}

std::string Block::getTexture() {
    if (getMass() == 0) {
        return "block.tga";
    }

    return "movable_block.tga";
}

void Block::draw(Level& level) {
    // utilisation de la texture
    sf::RenderWindow& window = level.getWindow();
    sprite.setTexture(level.getResourceManager().getTexture(getTexture()));

    // coloration du bloc selon sa charge
    if (getCharge() > 0) {
        sprite.setColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        sprite.setColor(sf::Color(255, 180, 180));
    } else {
        sprite.setColor(sf::Color::White);
    }

    sprite.setPosition(getPosition());
    window.draw(sprite);

    if (isSelected()) {
        select_sprite.setPosition(getPosition());
        select_sprite.setTexture(
            level.getResourceManager().getTexture("block_select.tga")
        );

        window.draw(select_sprite);
    }
}

void Block::activated(Level& level, Object* object) {
    // ne rien faire si le bloc est activé.
    // Ceci est un bloc de base qui n'a pas a réagir
    // aux activations
}

sf::FloatRect Block::getAABB() const {
    return sf::FloatRect(
        getPosition().x - Constants::GRID / 2,
        getPosition().y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    );
}

float Block::getRadius() const {
    return Constants::GRID / 2;
}

unsigned int Block::getTypeId() const {
    return TYPE_ID;
}

CollisionType Block::getCollisionType() const {
    return CollisionType::AABB;
}
