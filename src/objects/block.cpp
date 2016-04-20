#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/game.hpp"
#include "objects/player.hpp"
#include "objects/block.hpp"

const unsigned int Block::TYPE_ID = 2;

Block::Block() : Object() {
    sprite.setOrigin(sf::Vector2f(23, 23));
    aabb = sf::FloatRect(
        -Manager::GRID / 2,
        -Manager::GRID / 2,
        Manager::GRID,
        Manager::GRID
    );
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

void Block::draw(Level& level) {
    // récupération de la texture correspondant au type de bloc
    std::string texture_name = "block.tga";

    if (getMass() > 0) {
        texture_name = "movable_" + texture_name;
    }

    if (isSelected()) {
        texture_name = "selected_" + texture_name;
    }

    sprite.setTexture(*ResourceManager::get().getTexture(
        "objects/" + texture_name
    ));

    // coloration du bloc selon sa charge
    if (getCharge() > 0) {
        sprite.setColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        sprite.setColor(sf::Color(255, 180, 180));
    } else {
        sprite.setColor(sf::Color::White);
    }

    sprite.setPosition(getPosition());
    level.getManager().getWindow().draw(sprite);
}

void Block::activate(Game& game, Object::Ptr object) {
    // ne rien faire si le bloc est activé.
    // Ceci est un bloc de base qui n'a pas a réagir
    // aux activations
}

sf::FloatRect Block::getAABB() const {
    sf::FloatRect transl_aabb = aabb;
    transl_aabb.left += getPosition().x;
    transl_aabb.top += getPosition().y;

    return transl_aabb;
}

float Block::getRadius() const {
    return Manager::GRID / 2;
}

unsigned int Block::getTypeId() const {
    return TYPE_ID;
}

CollisionType Block::getCollisionType() const {
    return CollisionType::AABB;
}
