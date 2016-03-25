#include "collision.hpp"
#include "player.hpp"
#include "block.hpp"
#include "object.hpp"
#include <utility>
#include <iostream>

namespace Collision {
    // initialisation du dictionnaire associant les types
    // impliqués dans une collision à leur fonction de résolution
    collision_dispatcher dispatch = {
        {std::make_pair(Player::TYPE_ID, Block::TYPE_ID), &ballToBlock},
        {std::make_pair(Block::TYPE_ID, Player::TYPE_ID), &blockToBall},
        {std::make_pair(Player::TYPE_ID, Player::TYPE_ID), &ballToBall},
        {std::make_pair(Block::TYPE_ID, Block::TYPE_ID), &blockToBlock}
    };

    bool ballToBlock(Object& objA, Object& objB, sf::Vector2f& normal, float& depth) {
        Player player = dynamic_cast<Player&>(objA);
        Block block = dynamic_cast<Block&>(objB);

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
        if (!isInside && squaredLength >= ball.getRadius() * ball.getRadius()) {
            return false;
        }

        float length = std::sqrt(squaredLength);
        depth = ball.getRadius() - length;

        if (length != 0) {
            normal = prenormal / length;
        }

        if (isInside) {
            normal *= -1.f;
        }

        return true;
    }

    bool blockToBall(Object& objA, Object& objB, sf::Vector2f& normal, float& depth) {
        // la collision Block -> Ball est la collision Ball -> Block
        // avec une normale de collision retournée
        bool result = ballToBlock(objB, objA, normal, depth);
        normal *= -1.f;
        return result;
    }

    bool ballToBall(Object& objA, Object& objB, sf::Vector2f& normal, float& depth) {
        Ball ballA = dynamic_cast<Ball&>(objA);
        Ball ballB = dynamic_cast<Ball&>(objB);

        sf::Vector2f dir = ballB.getPosition() - ballA.getPosition();
        float squaredLength = dir.x * dir.x + dir.y * dir.y;
        float totalRadius = ballB.getRadius() + ballA.getRadius();

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

    bool blockToBlock(Object& objA, Object& objB, sf::Vector2f& normal, float& depth) {
        Block blockA = dynamic_cast<Block&>(objA);
        Block blockB = dynamic_cast<Block&>(objB);

        std::unique_ptr<sf::FloatRect> aabb = blockA.getAABB();
        std::unique_ptr<sf::FloatRect> obj_aabb = blockB.getAABB();
        sf::Vector2f relpos = blockA.getPosition() - blockB.getPosition();

        float overlap_x = aabb->width / 2 + obj_aabb->width / 2 - std::abs(relpos.x);
        float overlap_y = aabb->height / 2 + obj_aabb->height / 2 - std::abs(relpos.y);

        // si il n'y a pas de chauvauchement sur l'axe X et Y, pas de collision
        if (overlap_x <= 0 || overlap_y <= 0) {
            return false;
        }

        // on choisit l'axe de pénétration maximale pour calculer la normale
        if (overlap_x < overlap_y) {
            if (relpos.x < 0) {
                normal.x = -1;
            } else {
                normal.x = 1;
            }

            normal.y = 0;
            depth = overlap_x;
        } else {
            if (relpos.y < 0) {
                normal.y = -1;
            } else {
                normal.y = 1;
            }

            normal.x = 0;
            depth = overlap_y;
        }

        return true;
    }
}
