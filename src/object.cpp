#include "object.hpp"
#include "constants.hpp"

Object::Object(float x, float y) :
    acceleration(0, 0), velocity(0, 0), position(x, y),
    accelerationLine(sf::Lines, 2),
    velocityLine(sf::Lines, 2),
    mass(1.f), charge(0.f), layer(10) {}

sf::Vector2f Object::getForces(EngineState& state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, Constants::GRAVITY);

    return forces;
}

void Object::draw(sf::RenderWindow& window) {
    velocityLine[0].position = position;
    velocityLine[0].color = sf::Color::Green;
    velocityLine[1].position = position + velocity * 1.f;
    velocityLine[1].color = sf::Color::Green;

    accelerationLine[0].position = position;
    accelerationLine[0].color = sf::Color::Red;
    accelerationLine[1].position = position + acceleration * 1.f;
    accelerationLine[1].color = sf::Color::Red;

    window.draw(velocityLine);
    window.draw(accelerationLine);
}

void Object::update(EngineState& state) {
    // on représente les objets de masse infinie avec une
    // masse nulle. Ces objets ne sont pas déplaçables
    if (mass == 0) {
        acceleration.x = acceleration.y = 0;
        velocity.x = velocity.y = 0;
        return;
    }

    // intégration de la vitesse dans la position
    position += velocity * state.delta;

    // intégration des forces appliquées sur l'objet dans la vitesse
    acceleration = getForces(state) / mass;
    velocity += acceleration * state.delta;
}

sf::Vector2f Object::getAcceleration() {
    return acceleration;
}

sf::Vector2f Object::getVelocity() {
    return velocity;
}

void Object::setVelocity(sf::Vector2f set_velocity) {
    velocity = set_velocity;
}

sf::Vector2f Object::getPosition() {
    return position;
}

float Object::getMass() {
    return mass;
}

void Object::setMass(float set_mass) {
    mass = set_mass;
}

float Object::getCharge() {
    return charge;
}

void Object::setCharge(float set_charge) {
    charge = set_charge;
}

unsigned int Object::getLayer() {
    return layer;
}

void Object::setLayer(unsigned int set_layer) {
    layer = set_layer;
}

bool ObjectCompare::operator()(Object* const &t1, Object* const &t2) {
    return t1->getLayer() > t2->getLayer();
}
