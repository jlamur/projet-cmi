#include "physics_object.hpp"

void PhysicsObject::update(Engine engine, float delta) {
    sf::Vector2f forces = getForces(engine, objects);

    // TODO: intégrer le vecteur force dans la vitesse puis la position
}

sf::Vector2f PhysicsObject::getForces(State state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, Engine::GRAVITY);

    // TODO: collisions entre objets

    return forces;
}
