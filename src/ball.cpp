#include "ball.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>
#include <iostream>

Ball::Ball(float x, float y) : Object(x, y) {
    // déplacement de l'origine au centre de la balle
    sprite.setOrigin(sf::Vector2f(getRadius(), getRadius()));
}

sf::Vector2f Ball::getForces(EngineState& state) {
    sf::Vector2f forces = Object::getForces(state);

    // déplacement de la balle après appui sur les touches de direction
    if (state.keys[sf::Keyboard::Left]) {
        forces += sf::Vector2f(-Constants::MOVE, 0);
    }

    if (state.keys[sf::Keyboard::Right]) {
        forces += sf::Vector2f(Constants::MOVE, 0);
    }

    return forces;
}

void Ball::draw(sf::RenderWindow& window, ResourceManager& resources) {
    Object::draw(window, resources);

    // utilisation de la texture
	sprite.setTexture(resources.getTexture("ball.png"));

    // déplacement du sprite à la position de la balle
    sprite.rotate(getVelocity().x * .1f);
    sprite.setPosition(getPosition());
    window.draw(sprite);
}

std::unique_ptr<sf::FloatRect> Ball::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - getRadius(),
        getPosition().y - getRadius(),
        2 * getRadius(), 2 * getRadius()
    ));
}

bool Ball::getCollisionInfo(Object& obj, sf::Vector2f& normal, float& depth) {
    return obj.getCollisionInfo(*this, normal, depth);
}

bool Ball::getCollisionInfo(Ball& obj, sf::Vector2f& normal, float& depth) {
    sf::Vector2f dir = getPosition() - obj.getPosition();
    float squaredLength = dir.x * dir.x + dir.y * dir.y;
    float totalRadius = getRadius() + obj.getRadius();

    // si les deux balles sont à une distance supérieure
    // à la somme de leurs deux rayons, il n'y a pas eu collision
    if (squaredLength > totalRadius * totalRadius) {
        return false;
    }

    float length = std::sqrt(squaredLength);

    // les balles sont sur la même position.
    // Renvoie une normale apte à séparer les deux balles
    if (length == 0) {
        depth = totalRadius;
        normal.x = 0;
        normal.y = -1;
        return true;
    }

    // il y a eu collision
    depth = totalRadius - length;
    normal = dir / length;
    return true;
}

bool Ball::getCollisionInfo(Block& obj, sf::Vector2f& normal, float& depth) {
    // recherche du point le plus proche du centre de la
    // balle sur le bloc. On regarde la position relative
    // du cercle par rapport au bloc
    std::unique_ptr<sf::FloatRect> aabb = obj.getAABB();
    sf::Vector2f relpos = getPosition() - obj.getPosition();
    sf::Vector2f closest = relpos;

    // on restreint la position relative pour rester
    // à l'intérieur du bloc
    if (closest.x < -aabb->width / 2) {
        closest.x = -aabb->width / 2;
    }

    if (closest.x > aabb->width / 2) {
        closest.x = aabb->width / 2;
    }

    if (closest.y < -aabb->height / 2) {
        closest.y = -aabb->height / 2;
    }

    if (closest.y > aabb->height / 2) {
        closest.y = aabb->height / 2;
    }

    // si la position n'a pas été changée, elle
    // était déjà à l'intérieur du cercle : le cercle
    // est dans le bloc
    float isInside = false;

    if (relpos == closest) {
        isInside = true;

        // on se colle au bord le plus proche du bloc
        if (std::abs(relpos.x) > std::abs(relpos.y)) {
            if (closest.x > 0) {
                closest.x = aabb->width / 2;
            } else {
                closest.x = -aabb->width / 2;
            }
        } else {
            if (closest.y > 0) {
                closest.y = aabb->height / 2;
            } else {
                closest.y = -aabb->height / 2;
            }
        }
    }

    // la normale est portée par la direction
    // du point le plus proche au centre de la balle
    sf::Vector2f prenormal = relpos - closest;
    float squaredLength = prenormal.x * prenormal.x + prenormal.y * prenormal.y;

    // si la balle est à l'extérieur et que
    // la normale est plus longue que son rayon,
    // il n'y a pas collision
    if (!isInside && squaredLength >= getRadius() * getRadius()) {
        return false;
    }

    float length = std::sqrt(squaredLength);
    depth = getRadius() - length;

    if (length != 0) {
        normal = prenormal / length;
    }

    if (isInside) {
        normal *= -1.f;
    }

    return true;
}

float Ball::getRadius() {
    return 10 * getMass();
}
