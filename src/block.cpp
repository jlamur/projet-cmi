#include "block.hpp"
#include "ball.hpp"
#include "constants.hpp"

Block::Block(float x, float y) : Object(x, y),
    shape(sf::Vector2f(Constants::GRID, Constants::GRID)) {
    // par défaut, les blocs ne sont pas déplaçables
    // et ont donc une masse infinie
    setMass(0.f);

    shape.setOrigin(sf::Vector2f(Constants::GRID / 2, Constants::GRID / 2));
}

void Block::draw(sf::RenderWindow& window) {
    Object::draw(window);

	// chargement de la texture de test
	if (!texture.loadFromFile("./res/block.png")) {
    	// erreur
	}

	shape.setTexture(&texture);

    if (getCharge() > 0) {
        shape.setFillColor(sf::Color(180, 180, 255));
    } else if (getCharge() < 0) {
        shape.setFillColor(sf::Color(255, 180, 180));
    } else {
        shape.setFillColor(sf::Color::White);
    }

    shape.setPosition(getPosition());
    window.draw(shape);
}

std::unique_ptr<sf::FloatRect> Block::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - Constants::GRID / 2,
        getPosition().y - Constants::GRID / 2,
        Constants::GRID, Constants::GRID
    ));
}

bool Block::getCollisionInfo(Object& obj, sf::Vector2f& normal, float& depth) {
    return obj.getCollisionInfo(*this, normal, depth);
}

bool Block::getCollisionInfo(Ball& obj, sf::Vector2f& normal, float& depth) {
    // la collision Block -> Ball est la collision Ball -> Block
    // avec une normale de collision retournée
    bool result = obj.getCollisionInfo(*this, normal, depth);
    normal *= -1.f;
    return result;
}

bool Block::getCollisionInfo(Block& obj, sf::Vector2f& normal, float& depth) {
    std::unique_ptr<sf::FloatRect> aabb = getAABB();
    std::unique_ptr<sf::FloatRect> obj_aabb = obj.getAABB();
    sf::Vector2f relpos = getPosition() - obj.getPosition();

    float overlap_x = aabb->width / 2 + obj_aabb->width / 2 - std::abs(relpos.x);
    float overlap_y = aabb->height / 2 + obj_aabb->height / 2 - std::abs(relpos.y);

    // si il n'y a pas de chauvauchement sur l'axe X et Y, pas de collision
    if (overlap_x <= 0 || overlap_y <= 0) {
        return false;
    }

    // on choisit l'axe de pénétration maximale pour calculer la normale
    if (overlap_x < overlap_y) {
        if (relpos.x < 0) {
            normal.x = -1;
        } else {
            normal.x = 1;
        }

        normal.y = 0;
        depth = overlap_x;
    } else {
        if (relpos.y < 0) {
            normal.y = -1;
        } else {
            normal.y = 1;
        }

        normal.x = 0;
        depth = overlap_y;
    }

    return true;
}
