#ifndef __PTF_COLLISION_HPP__
#define __PTF_COLLISION_HPP__

#include <SFML/Graphics.hpp>

class Object;

/**
 * Type de collision : entre deux rectangles ou
 * entre deux cercles
 */
enum class CollisionType {AABB, CIRCLE};

/**
 * Structure qui retient les informations
 * sur les collisions
 */
struct CollisionData {
    sf::Vector2f normal;
    float depth;

    Object& obj_a;
    Object& obj_b;

    CollisionData(Object& obj_a, Object& obj_b);
};

/**
 * Détermine les informations sur la collision
 * entre les deux objets donnés dans data et stocke
 * ces informations dans data
 */
bool getCollisionData(CollisionData& data);

#endif
