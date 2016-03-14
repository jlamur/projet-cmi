#include "physics_object.hpp"

PhysicsObject::PhysicsObject(float x, float y) :
    Object(x, y), mass(1), accelLine(sf::LinesStrip, 2),
    velLine(sf::LinesStrip, 2) {}

void PhysicsObject::draw(sf::RenderWindow& window) {
    velLine[0].position = position;
    velLine[0].color = sf::Color::Green;
    velLine[1].position = position + velocity * 1.f;
    velLine[1].color = sf::Color::Green;

    accelLine[0].position = position;
    accelLine[0].color = sf::Color::Red;
    accelLine[1].position = position + acceleration * 1.f;
    accelLine[1].color = sf::Color::Red;

    window.draw(velLine);
    window.draw(accelLine);
}

void PhysicsObject::update(EngineState& state) {
    // intégration de la vitesse dans la position
    position += velocity * state.delta;

    // intégration des forces appliquées sur l'objet dans la vitesse
    acceleration = getForces(state) / mass;
    velocity += acceleration * state.delta;
}

sf::Vector2f PhysicsObject::getForces(EngineState& state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, PhysicsObject::GRAVITY);

    return forces;
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
