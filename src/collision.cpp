#include "collision.hpp"
#include "player.hpp"
#include "block.hpp"
#include "object.hpp"
#include <functional>
#include <utility>

/**
 * Détermination des informations sur une collision entre
 * un joueur et un bloc (normale et profondeur de collision)
 */
bool playerToBlock(CollisionData& data) {
    Player player = dynamic_cast<Player&>(data.objA);
    Block block = dynamic_cast<Block&>(data.objB);

    // recherche du point le plus proche du centre de la
    // balle sur le bloc. On regarde la position relative
    // du cercle par rapport au bloc
    std::unique_ptr<sf::FloatRect> aabb = block.getAABB();
    sf::Vector2f relpos = block.getPosition() - player.getPosition();
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
    if (!isInside && squaredLength >= player.getRadius() * player.getRadius()) {
        return false;
    }

    float length = std::sqrt(squaredLength);
    data.depth = player.getRadius() - length;

    if (length != 0) {
        data.normal = prenormal / length;
    }

    if (isInside) {
        data.normal *= -1.f;
    }

    return true;
}

/**
 * Détermination des informations sur une collision entre
 * un bloc et un joueur (normale et profondeur de collision)
 */
bool blockToPlayer(CollisionData& data) {
    // la collision Block -> Player est la collision Player -> Block
    Object& objT = data.objB;
    data.objB = data.objA;
    data.objA = objT;

    return playerToBlock(data);
}

/**
 * Détermination des informations sur une collision entre
 * deux joueurs (normale et profondeur de collision)
 */
bool playerToPlayer(CollisionData& data) {
    Player playerA = dynamic_cast<Player&>(data.objA);
    Player playerB = dynamic_cast<Player&>(data.objB);

    sf::Vector2f dir = playerB.getPosition() - playerA.getPosition();
    float squaredLength = dir.x * dir.x + dir.y * dir.y;
    float totalRadius = playerB.getRadius() + playerA.getRadius();

    // si les deux balles sont à une distance supérieure
    // à la somme de leurs deux rayons, il n'y a pas eu collision
    if (squaredLength > totalRadius * totalRadius) {
        return false;
    }

    float length = std::sqrt(squaredLength);

    // les balles sont sur la même position.
    // Renvoie une normale apte à séparer les deux balles
    if (length == 0) {
        data.depth = totalRadius;
        data.normal.x = 0;
        data.normal.y = -1;
        return true;
    }

    // il y a eu collision
    data.depth = totalRadius - length;
    data.normal = dir / length;
    return true;
}

/**
 * Détermination des informations sur une collision entre
 * deux blocs (normale et profondeur de collision)
 */
bool blockToBlock(CollisionData& data) {
    Block blockA = dynamic_cast<Block&>(data.objA);
    Block blockB = dynamic_cast<Block&>(data.objB);

    std::unique_ptr<sf::FloatRect> aabb = blockA.getAABB();
    std::unique_ptr<sf::FloatRect> obj_aabb = blockB.getAABB();
    sf::Vector2f relpos = blockB.getPosition() - blockA.getPosition();

    float overlap_x = aabb->width / 2 + obj_aabb->width / 2 - std::abs(relpos.x);
    float overlap_y = aabb->height / 2 + obj_aabb->height / 2 - std::abs(relpos.y);

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
    std::pair<unsigned int, unsigned int>,
    std::function<bool(CollisionData&)>
> collision_map = {
    {std::make_pair(Player::TYPE_ID, Block::TYPE_ID), playerToBlock},
    {std::make_pair(Block::TYPE_ID, Player::TYPE_ID), blockToPlayer},
    {std::make_pair(Player::TYPE_ID, Player::TYPE_ID), playerToPlayer},
    {std::make_pair(Block::TYPE_ID, Block::TYPE_ID), blockToBlock}
};

CollisionData::CollisionData(Object& objA, Object& objB) :
    objA(objA), objB(objB) {}

bool getCollisionData(CollisionData& data) {
    return collision_map[std::make_pair(
        data.objA.getTypeId(),
        data.objB.getTypeId()
    )](data);
}
