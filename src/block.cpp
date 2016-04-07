#include "block.hpp"
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

ObjectPtr Block::clone() const {
    return ObjectPtr(new Block(*this));
}

ObjectPtr Block::load(std::ifstream& file) {
    ObjectPtr object = ObjectPtr(new Block);

    // lecture des propriétés communes des objets
    Object::load(file, object);
    file.seekg(1, file.cur);

    return object;
}

void Block::draw(Manager& manager) {
    // utilisation de la texture
    ResourceManager& resources = manager.getResourceManager();
    sf::RenderWindow& window = manager.getWindow();

    sprite.setTexture(resources.getTexture("block.png"));
    select_sprite.setTexture(resources.getTexture("block_select.png"));

    // coloration du bloc en fonction de sa charge
    if (getCharge() > 0) {
        sprite.setColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        sprite.setColor(sf::Color(255, 180, 180));
    } else {
        sprite.setColor(sf::Color::White);
    }

    sprite.setPosition(getPosition());
    select_sprite.setPosition(getPosition());

    window.draw(sprite);

    if (isSelected()) {
        window.draw(select_sprite);
    }
}

std::unique_ptr<sf::FloatRect> Block::getAABB() const {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - Constants::GRID / 2,
        getPosition().y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    ));
}

unsigned int Block::getTypeId() const {
    return TYPE_ID;
}
