#include "block.hpp"
#include "level.hpp"
#include "player.hpp"
#include "constants.hpp"
#include "resource_manager.hpp"

const unsigned int Block::TYPE_ID = 2;

Block::Block() : Object() {
    aabb = sf::FloatRect(
        -Constants::GRID / 2,
        -Constants::GRID / 2,
        Constants::GRID,
        Constants::GRID
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

void Block::prepareDraw(ResourceManager& resources) {
    std::string texture_name = "movable_block.tga";

    if (getMass() == 0) {
        texture_name = "block.tga";
    }

    sprite.setTexture(resources.getTexture(texture_name));
    sprite.setOrigin(sf::Vector2f(23, 23));

    select_sprite.setTexture(resources.getTexture("block_select.tga"));
    select_sprite.setOrigin(sf::Vector2f(23, 23));
}

void Block::draw(Level& level) {
    // utilisation de la texture
    sf::RenderWindow& window = level.getWindow();
    prepareDraw(level.getResourceManager());

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
        window.draw(select_sprite);
    }
}

void Block::activate(Level& level, Object* object) {
    // ne rien faire si le bloc est activé.
    // Ceci est un bloc de base qui n'a pas a réagir
    // aux activations
}

void Block::kill(Game& game) {
    // rien à faire de plus si le bloc a été tué
}

sf::FloatRect Block::getAABB() const {
    sf::FloatRect transl_aabb = aabb;
    transl_aabb.left += getPosition().x;
    transl_aabb.top += getPosition().y;

    return transl_aabb;
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
