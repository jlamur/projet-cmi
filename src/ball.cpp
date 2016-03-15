#include "ball.hpp"
#include "block.hpp"
#include "constants.hpp"
#include <array>

Ball::Ball(float x, float y) : Object(x, y), shape(10) {
    shape.setOrigin(sf::Vector2f(10, 10));
}

sf::Vector2f Ball::getForces(EngineState& state) {
    sf::Vector2f forces = Object::getForces(state);

    // déplacement de la balle après appui sur les touches de direction
    if (state.keys[sf::Keyboard::Left]) {
        forces += sf::Vector2f(-Constants::MOVE, 0);
    }

    if (state.keys[sf::Keyboard::Right]) {
        forces += sf::Vector2f(Constants::MOVE, 0);
    }

    // force d'attraction entre les balles et les blocs chargés
    if (getCharge() != 0) {
        for (unsigned int j = 0; j < state.objects.size(); j++) {
            Object *attractive = state.objects[j];

            if (attractive == this || attractive->getCharge() == 0) {
                continue;
            }

            // vecteur allant de l'objet attirant vers l'objet considéré
            sf::Vector2f attraction(getPosition() - attractive->getPosition());

            // la norme de ce vecteur est la distance entre les objets
            float distanceSquared = attraction.x * attraction.x +
                attraction.y * attraction.y;

            // éviter la division par zéro
            if (distanceSquared == 0) {
                continue;
            }

            // normalisation du vecteur direction qui porte
            // la force d'attraction, puis application de la norme
            attraction /= std::sqrt(distanceSquared);
            attraction *= Constants::ATTRACTION * (
                (getCharge() * attractive->getCharge()) /
                distanceSquared
            );

            forces += attraction;
        }
    }

    return forces;
}

void Ball::draw(sf::RenderWindow& window) {
    Object::draw(window);

	// chargement de la texture de test
	if (!texture.loadFromFile("./res/ball.png")) {
    	// erreur
	}

	shape.setTexture(&texture);
    shape.setPosition(getPosition());
    window.draw(shape);
}

std::unique_ptr<sf::FloatRect> Ball::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        getPosition().x - 10,
        getPosition().y - 10,
        20, 20
    ));
}

bool Ball::getNormal(Object& obj, sf::Vector2f& normal) {
    return obj.getNormal(*this, normal);
}

bool Ball::getNormal(Ball& obj, sf::Vector2f& normal) {
    sf::Vector2f dir = obj.getPosition() - getPosition();
    float squaredLength = dir.x * dir.x + dir.y * dir.y;

    // TODO: supprimer les valeurs magiques
    // si les deux balles sont à une distance supérieure
    // à la somme de leurs deux rayons, il n'y a pas eu collision
    if (squaredLength > 20 * 20) {
        return false;
    }

    float length = std::sqrt(squaredLength);

    // les balles sont sur la même position.
    // Renvoie une normale apte à séparer les deux balles
    if (length == 0) {
        normal.x = 0;
        normal.y = -1;
        return true;
    }

    // il y a eu collision
    normal = dir / length;
    return true;
}

bool Ball::getNormal(Block& obj, sf::Vector2f& normal) {
    // recherche du point le plus proche du centre de la
    // balle sur le bloc. On regarde la position relative
    // du cercle par rapport au bloc
    std::unique_ptr<sf::FloatRect> aabb = obj.getAABB();
    sf::Vector2f relpos = getPosition() - obj.getPosition();
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

    // TODO: supprimer les valeurs magiques
    // si la balle est à l'extérieur et que
    // la normale est plus longue que son rayon,
    // il n'y a pas collision
    if (!isInside && squaredLength > 20 * 20) {
        return false;
    }

    normal = prenormal / std::sqrt(squaredLength);

    if (isInside) {
        normal *= -1.f;
    }

    return true;
}
