#include "collision.hpp"
#include "player.hpp"
#include "block.hpp"
#include "gravity_block.hpp"
#include "object.hpp"
#include <functional>
#include <utility>
#include <cmath>

/**
 * Détermination des informations sur une collision entre
 * un cercle et un rectangle
 */
bool circleToAABB(CollisionData& data) {
    Object& circle = data.obj_a;
    Object& aabb = data.obj_b;

    // recherche du point le plus proche du centre du cercle
    // sur le rectangle. On regarde la position relative du cercle
    // par rapport au rectangle
    sf::FloatRect box = aabb.getAABB();
    sf::Vector2f relpos = aabb.getPosition() - circle.getPosition();
    sf::Vector2f closest = relpos;

    // on restreint la position relative pour rester
    // à l'intérieur du rectangle
    if (closest.x < -box.width / 2) {
        closest.x = -box.width / 2;
    }

    if (closest.x > box.width / 2) {
        closest.x = box.width / 2;
    }

    if (closest.y < -box.height / 2) {
        closest.y = -box.height / 2;
    }

    if (closest.y > box.height / 2) {
        closest.y = box.height / 2;
    }

    // si la position n'a pas été changée, elle était déjà
    // à l'intérieur du cercle : le cercle est dans le rectangle
    float is_inside = false;

    if (relpos == closest) {
        is_inside = true;

        // on se colle au bord le plus proche du rectangle
        if (std::abs(relpos.x) > std::abs(relpos.y)) {
            if (closest.x > 0) {
                closest.x = box.width / 2;
            } else {
                closest.x = -box.width / 2;
            }
        } else {
            if (closest.y > 0) {
                closest.y = box.height / 2;
            } else {
                closest.y = -box.height / 2;
            }
        }
    }

    // la normale est portée par la direction
    // du point le plus proche au centre du cercle
    sf::Vector2f prenormal = relpos - closest;
    float squared_length = prenormal.x * prenormal.x + prenormal.y * prenormal.y;

    // si le cercle est à l'extérieur et que la normale est plus
    // longue que son rayon, il n'y a pas collision
    if (!is_inside && squared_length >= circle.getRadius() * circle.getRadius()) {
        return false;
    }

    float length = std::sqrt(squared_length);
    data.depth = circle.getRadius() - length;

    if (length != 0) {
        data.normal = prenormal / length;
    }

    if (is_inside) {
        data.normal *= -1.f;
    }

    return true;
}

/**
 * Détermination des informations sur une collision entre
 * un rectangle et un cercle
 */
bool AABBToCircle(CollisionData& data) {
    // la collision rectangle -> cercle est la collision cercle -> rectangle
    Object& transfer = data.obj_b;
    data.obj_b = data.obj_a;
    data.obj_a = transfer;

    return circleToAABB(data);
}

/**
 * Détermination des informations sur une collision entre
 * deux cercles
 */
bool circleToCircle(CollisionData& data) {
    Object& circle_a = data.obj_a;
    Object& circle_b = data.obj_b;

    sf::Vector2f dir = circle_b.getPosition() - circle_a.getPosition();
    float squared_length = dir.x * dir.x + dir.y * dir.y;
    float total_radius = circle_b.getRadius() + circle_a.getRadius();

    // si les deux cercles sont à une distance supérieure
    // à la somme de leurs deux rayons, il n'y a pas eu collision
    if (squared_length > total_radius * total_radius) {
        return false;
    }

    float length = std::sqrt(squared_length);

    // les cercles sont sur la même position.
    // Renvoie une normale apte à séparer les deux cercles
    if (length == 0) {
        data.depth = total_radius;
        data.normal.x = 0;
        data.normal.y = -1;
        return true;
    }

    // il y a eu collision
    data.depth = total_radius - length;
    data.normal = dir / length;
    return true;
}

/**
 * Détermination des informations sur une collision entre
 * deux rectangles
 */
bool AABBToAABB(CollisionData& data) {
    Object& aabb_a = data.obj_a;
    Object& aabb_b = data.obj_b;

    sf::FloatRect box_a = aabb_a.getAABB();
    sf::FloatRect box_b = aabb_b.getAABB();
    sf::Vector2f relpos = aabb_b.getPosition() - aabb_a.getPosition();

    float overlap_x = box_a.width / 2 + box_b.width / 2 - std::abs(relpos.x);
    float overlap_y = box_a.height / 2 + box_b.height / 2 - std::abs(relpos.y);

    // si il n'y a pas de chauvauchement sur l'axe X et Y, pas de collision
    if (overlap_x <= 0 || overlap_y <= 0) {
        return false;
    }

    // on choisit l'axe de pénétration maximale pour calculer la normale
    if (overlap_x < overlap_y) {
        if (relpos.x < 0) {
            data.normal.x = -1;
        } else {
            data.normal.x = 1;
        }

        data.normal.y = 0;
        data.depth = overlap_x;
    } else {
        if (relpos.y < 0) {
            data.normal.y = -1;
        } else {
            data.normal.y = 1;
        }

        data.normal.x = 0;
        data.depth = overlap_y;
    }

    return true;
}

/**
 * Dictionnaire associant les types impliqués
 * dans une collision à leur fonction de résolution
 */
std::map<
    std::pair<CollisionType, CollisionType>,
    std::function<bool(CollisionData&)>
> collision_map = {
    {std::make_pair(CollisionType::CIRCLE, CollisionType::CIRCLE), circleToCircle},
    {std::make_pair(CollisionType::CIRCLE, CollisionType::AABB), circleToAABB},
    {std::make_pair(CollisionType::AABB, CollisionType::AABB), AABBToAABB},
    {std::make_pair(CollisionType::AABB, CollisionType::CIRCLE), AABBToCircle}
};

CollisionData::CollisionData(Object& obj_a, Object& obj_b) : obj_a(obj_a), obj_b(obj_b) {}
bool getCollisionData(CollisionData& data) {
    return collision_map[std::make_pair(
        data.obj_a.getCollisionType(),
        data.obj_b.getCollisionType()
    )](data);
}
