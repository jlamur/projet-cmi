#ifndef __PTF_COLLISION_HPP__
#define __PTF_COLLISION_HPP__

#include "object.hpp"
#include "collision_data.hpp"
#include <SFML/Graphics.hpp>
#include <utility>
#include <functional>

namespace Collision {
    extern std::map<
        std::pair<unsigned int, unsigned int>,
        std::function<bool(CollisionData&)>
    > dispatch;

    bool playerToBlock(CollisionData& data);
    bool blockToPlayer(CollisionData& data);
    bool playerToPlayer(CollisionData& data);
    bool blockToBlock(CollisionData& data);
}

#endif
