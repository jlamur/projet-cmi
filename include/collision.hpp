#ifndef __PTF_COLLISION_HPP__
#define __PTF_COLLISION_HPP__

#include "object.hpp"
#include "collision_data.hpp"
#include <SFML/Graphics.hpp>
#include <utility>

namespace Collision {
    typedef bool (*collision_detect)(CollisionData&);
    typedef std::map<std::pair<unsigned int, unsigned int>, collision_detect> collision_dispatcher;
    extern collision_dispatcher dispatch;

    bool playerToBlock(CollisionData& data);
    bool blockToPlayer(CollisionData& data);
    bool playerToPlayer(CollisionData& data);
    bool blockToBlock(CollisionData& data);
}

#endif
