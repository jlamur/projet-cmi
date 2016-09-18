#include <cmath>
#include <SFML/Graphics.hpp>
#include "objects/object.hpp"
#include "objects/geometry.hpp"

Geometry::Geometry(sf::Vector2f position, float radius) :
    type(Geometry::Type::CIRCLE), position(position), radius(radius),
    rect(sf::FloatRect(
        position.x - radius, position.y - radius,
        2 * radius, 2 * radius
    )) {}

Geometry::Geometry(sf::Vector2f position, sf::Vector2f size) :
    type(Geometry::Type::RECTANGLE), position(position),
    rect(sf::FloatRect(position, size)) {}

bool Geometry::intersects(Geometry other) {
    return rect.intersects(other.rect);
}

bool Geometry::collides(Geometry other, sf::Vector2f& normal, float& depth) {
    if (type == Geometry::Type::CIRCLE && other.type == Geometry::Type::CIRCLE) {
        return circleToCircle(other, normal, depth);
    }

    if (type == Geometry::Type::CIRCLE && other.type == Geometry::Type::RECTANGLE) {
        return circleToRectangle(other, normal, depth);
    }

    if (type == Geometry::Type::RECTANGLE && other.type == Geometry::Type::CIRCLE) {
        return rectangleToCircle(other, normal, depth);
    }

    return rectangleToRectangle(other, normal, depth);
}

/**
 * Détermination des informations sur une collision entre
 * un cercle et un rectangle
 */
bool Geometry::circleToRectangle(Geometry other, sf::Vector2f& normal, float& depth) {
    // recherche du point le plus proche du centre du cercle
    // sur le rectangle. On regarde la position relative du cercle
    // par rapport au rectangle
    sf::FloatRect box = other.rect;
    sf::Vector2f relpos = other.position - position;
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
    if (!is_inside && squared_length >= radius * radius) {
        return false;
    }

    float length = std::sqrt(squared_length);
    depth = radius - length;

    if (length != 0) {
        normal = prenormal / length;
    }

    if (is_inside) {
        normal *= -1.f;
    }

    return true;
}

bool Geometry::rectangleToCircle(Geometry other, sf::Vector2f& normal, float& depth) {
    // la collision rectangle -> cercle est la collision cercle <- rectangle
    return other.circleToRectangle(*this, normal, depth);
}

bool Geometry::circleToCircle(Geometry other, sf::Vector2f& normal, float& depth) {
    sf::Vector2f dir = other.position - position;
    float squared_length = dir.x * dir.x + dir.y * dir.y;
    float total_radius = other.radius + radius;

    // si les deux cercles sont à une distance supérieure
    // à la somme de leurs deux rayons, il n'y a pas eu collision
    if (squared_length > total_radius * total_radius) {
        return false;
    }

    float length = std::sqrt(squared_length);

    // les cercles sont sur la même position.
    // Renvoie une normale apte à séparer les deux cercles
    if (length == 0) {
        depth = total_radius;
        normal.x = 0;
        normal.y = -1;
        return true;
    }

    // il y a eu collision
    depth = total_radius - length;
    normal = dir / length;
    return true;
}

/**
 * Détermination des informations sur une collision entre
 * deux rectangles
 */
bool Geometry::rectangleToRectangle(Geometry other, sf::Vector2f& normal, float& depth) {
    sf::FloatRect box_a = rect;
    sf::FloatRect box_b = other.rect;
    sf::Vector2f relpos = other.position - position;

    float overlap_x = box_a.width / 2 + box_b.width / 2 - std::abs(relpos.x);
    float overlap_y = box_a.height / 2 + box_b.height / 2 - std::abs(relpos.y);

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
