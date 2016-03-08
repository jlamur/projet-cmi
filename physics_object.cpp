#include "physics_object.hpp"

void PhysicsObject::update(State state) {
    sf::Vector2f forces = getForces(state);

    // TODO: intégrer le vecteur force dans la vitesse puis la position
}

sf::Vector2f PhysicsObject::getForces(State state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, PhysicsObject::GRAVITY);

    // TODO: collisions entre objets

    return forces;
}
