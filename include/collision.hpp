#ifndef __PTF_COLLISION_HPP__
#define __PTF_COLLISION_HPP__

#include "object.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <string>

namespace Collision {
    typedef bool (*collision_data)(Object&, Object&, sf::Vector2f&, float&);
    typedef std::map<std::pair<unsigned int, unsigned int>, collision_data> collision_dispatcher;
    extern collision_dispatcher dispatch;

    bool playerToBlock(Object& objA, Object& objB, sf::Vector2f& normal, float& depth);
    bool blockToPlayer(Object& objA, Object& objB, sf::Vector2f& normal, float& depth);
    bool playerToPlayer(Object& objA, Object& objB, sf::Vector2f& normal, float& depth);
    bool blockToBlock(Object& objA, Object& objB, sf::Vector2f& normal, float& depth);
}

#endif
