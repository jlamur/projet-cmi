#include "ball.hpp"
#include "constants.hpp"

Ball::Ball(float x, float y) : Object(x, y), shape(10) {
    shape.setOrigin(sf::Vector2f(10, 10));
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
