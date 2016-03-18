#include "object.hpp"
#include "constants.hpp"

Object::Object(float x, float y) :
    acceleration(0, 0), velocity(0, 0), position(x, y),
    accelerationLine(sf::Lines, 2),
    velocityLine(sf::Lines, 2),
    mass(Constants::DEFAULT_MASS),
    inv_mass(1.f / Constants::DEFAULT_MASS),
    charge(Constants::DEFAULT_CHARGE),
    restitution(Constants::DEFAULT_RESTITUTION),
    layer(Constants::DEFAULT_LAYER) {}

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

bool Object::getCollisionInfo(Object& obj, sf::Vector2f& normal, float& depth) {
    return obj.getCollisionInfo(*this, normal, depth);
}

void Object::collide(Object& obj) {
    // si les objets ne sont pas sur la même couche,
    // ils ne peuvent pas entrer en collision
    if (getLayer() != obj.getLayer()) {
        return;
    }

    // si les deux boîtes englobantes des deux objets, ne
    // s'intersectent pas, il ne risque pas d'y avoir de collision
    if (!getAABB()->intersects(*obj.getAABB())) {
        return;
    }

    sf::Vector2f normal;
    float depth;

    // vérifie plus finement s'il y a collision et récupère
    // les informations sur la collision (normale et profondeur)
    if (!getCollisionInfo(obj, normal, depth)) {
        return;
    }

    sf::Vector2f codir = obj.getVelocity() - getVelocity();
    float dotnormal = codir.x * normal.x + codir.y * normal.y;

    // si les directions sont divergentes, pas besoin
    // de résoudre la collision
    if (dotnormal >= 0) {
        return;
    }

    // calcule et applique l'impulsion de résolution de la collision
    float restitution = std::min(getRestitution(), obj.getRestitution());
    float impulse = (-(1 + restitution) * dotnormal) /
        (getMassInvert() + obj.getMassInvert());

    setVelocity(getVelocity() - getMassInvert() * impulse * normal);
    obj.setVelocity(obj.getVelocity() + obj.getMassInvert() * impulse * normal);

    // correction de la position des objets. En raison de l'imprécision
    // des flottants sur la machine, les objets peuvent accumuler une
    // erreur de positionnement qui les fait "plonger" les un dans les autres.
    if (depth <= Constants::CORRECTION_THRESHOLD) {
        return;
    }

    sf::Vector2f positionCorrection = depth / (getMassInvert() +
        obj.getMassInvert()) * Constants::CORRECTION_PERCENTAGE * normal;

    setPosition(getPosition() - getMassInvert() * positionCorrection);
    obj.setPosition(obj.getPosition() + obj.getMassInvert() * positionCorrection);
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

void Object::setPosition(sf::Vector2f set_position) {
    position = set_position;
}

float Object::getMass() {
    return mass;
}

float Object::getMassInvert() {
    if (inv_mass >= 0) {
        return inv_mass;
    }

    if (mass == 0) {
        inv_mass = 0;
        return inv_mass;
    }

    inv_mass = 1 / mass;
    return inv_mass;
}

void Object::setMass(float set_mass) {
    mass = set_mass;
    inv_mass = -1.f;
}

float Object::getCharge() {
    return charge;
}

void Object::setCharge(float set_charge) {
    charge = set_charge;
}

float Object::getRestitution() {
    return restitution;
}

void Object::setRestitution(float set_restitution) {
    restitution = set_restitution;
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
