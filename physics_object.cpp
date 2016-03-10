#include "physics_object.hpp"

void PhysicsObject::update(State state) {
    sf::Vector2f acceleration = getForces(state) / mass;
    velocity += acceleration * state.delta;
    position += velocity * state.delta;
}

sf::Vector2f PhysicsObject::getVelocity() {
    return velocity;
}

void PhysicsObject::setVelocity(sf::Vector2f set_velocity) {
    velocity = set_velocity;
}

int PhysicsObject::getMass() {
    return mass;
}

void PhysicsObject::setMass(int set_mass) {
    mass = set_mass;
}

sf::Vector2f PhysicsObject::getForces(State state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, PhysicsObject::GRAVITY);

    // TODO: collisions entre objets

    return forces;
}
