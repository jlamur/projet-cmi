#ifndef __PTF_COLLISION_HPP__
#define __PTF_COLLISION_HPP__

#include <SFML/Graphics.hpp>

class Object;

/**
 * Structure qui retient des informations
 * sur les collisions
 */
struct CollisionData {
    sf::Vector2f normal;
    float depth;

    Object& objA;
    Object& objB;

    CollisionData(Object& objA, Object& objB);
};

/**
 * Détermine les informations sur la collision
 * entre les deux objets donnés dans data et
 * les stocke dans data
 */
bool getCollisionData(CollisionData& data);

#endif
