#ifndef __SKIZZLE_COLLISION_HPP__
#define __SKIZZLE_COLLISION_HPP__

#include <SFML/Graphics.hpp>
#include <memory>

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

    std::shared_ptr<Object> obj_a;
    std::shared_ptr<Object> obj_b;

    CollisionData();
};

/**
 * Détermine les informations sur la collision
 * entre les deux objets donnés dans data et stocke
 * ces informations dans data
 */
bool getCollisionData(CollisionData& data);

#endif
