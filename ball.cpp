#include "ball.hpp"

void Ball::draw(sf::RenderWindow& window) {
    shape.setPosition(position);
    window.draw(shape);
}

sf::Vector2f Ball::getForces(State state) {
    sf::Vector2f forces = PhysicsObject::getForces(state);

    // déplacement de la balle après appui sur les touches de direction
    if (state.goLeftKey) {
        forces += sf::Vector2f(-Ball::MOVE, 0);
    }

    if (state.goRightKey) {
        forces += sf::Vector2f(Ball::MOVE, 0);
    }

    // force d'attraction entre les balles et les blocs chargés
    if (getCharge() != 0) {
        for (unsigned int j = 0; j < state.objects.size(); j++) {
            Object *attractive = state.objects[j];

            if (attractive == this || attractive->getCharge() == 0) {
                continue;
            }

            // vecteur allant de l'objet attirant vers l'objet considéré
            sf::Vector2f attraction(attractive->getPosition() - position);

            // la norme de ce vecteur est la distance entre les objets
            float distanceSquared = attraction.x * attraction.x +
                attraction.y * attraction.y;

            // normalisation du vecteur direction qui porte
            // la force d'attraction, puis application de la norme
            attraction /= std::sqrt(distanceSquared);
            attraction *= Ball::ATTRACTION * (
                (charge * attractive->getCharge()) /
                distanceSquared
            );

            forces += attraction;
        }
    }

    return forces;
}
