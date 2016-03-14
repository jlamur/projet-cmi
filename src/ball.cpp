#include "ball.hpp"
#include "constants.hpp"

Ball::Ball(float x, float y) : PhysicsObject(x, y), shape(10 * mass) {
    shape.setOrigin(sf::Vector2f(10 * mass, 10 * mass));
}

void Ball::draw(sf::RenderWindow& window) {
    PhysicsObject::draw(window);

	// chargement de la texture de test
	if (!texture.loadFromFile("./res/ball.png")) {
    	// erreur
	}

	shape.setTexture(&texture);

    shape.setPosition(position);
    window.draw(shape);
}

std::unique_ptr<sf::FloatRect> Ball::getAABB() {
    return std::unique_ptr<sf::FloatRect>(new sf::FloatRect(
        position.x - 10 * mass,
        position.y - 10 * mass,
        20 * mass, 20 * mass
    ));
}

unsigned int Ball::getLayer() {
    return 1;
}

sf::Vector2f Ball::getForces(EngineState& state) {
    sf::Vector2f forces = PhysicsObject::getForces(state);

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
            sf::Vector2f attraction(position - attractive->getPosition());

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
                (charge * attractive->getCharge()) /
                distanceSquared
            );

            forces += attraction;
        }
    }

    return forces;
}
