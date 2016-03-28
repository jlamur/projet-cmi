#ifndef __PTF_COLLISION_DATA_HPP__
#define __PTF_COLLISION_DATA_HPP__

#include <SFML/Graphics.hpp>

class Object;

/**
 * Structure qui retient des informations sur les
 * collisions
 */
struct CollisionData {
    sf::Vector2f normal;
    float depth;

    Object& objA;
    Object& objB;

    CollisionData(Object& objA, Object& objB) : objA(objA), objB(objB) {}
};

#endif
