#include "object.hpp"
#include "constants.hpp"
#include "collision.hpp"
#include "collision_data.hpp"

Object::Object(float x, float y) :
    acceleration(0, 0), velocity(0, 0), position(x, y),
    acceleration_line(sf::Lines, 2),
    velocity_line(sf::Lines, 2),
    mass(Constants::DEFAULT_MASS),
    inv_mass(1.f / Constants::DEFAULT_MASS),
    charge(Constants::DEFAULT_CHARGE),
    restitution(Constants::DEFAULT_RESTITUTION),
    static_friction(0.4f),
    dynamic_friction(0.2f),
    layer(Constants::DEFAULT_LAYER) {}

sf::Vector2f Object::getForces(EngineState& state) {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, getMass() * Constants::GRAVITY);

    // force d'attraction entre objets chargés
    if (getCharge() != 0) {
        for (unsigned int j = 0; j < state.objects.size(); j++) {
            Object *attractive = state.objects[j];

            if (attractive == this || attractive->getCharge() == 0) {
                continue;
            }

            // vecteur allant de l'objet attracteur vers l'objet actuel
            sf::Vector2f attraction(getPosition() - attractive->getPosition());

            // la norme de ce vecteur est la distance entre les objets
            float distance_squared = attraction.x * attraction.x +
                attraction.y * attraction.y;

            // éviter la division par zéro
            if (distance_squared == 0) {
                continue;
            }

            // normalisation du vecteur direction qui porte
            // la force d'attraction, puis application de la norme
            attraction /= std::sqrt(distance_squared);
            attraction *= Constants::ATTRACTION * (
                (getCharge() * attractive->getCharge()) /
                distance_squared
            );

            forces += attraction;
        }
    }

    return forces;
}

void Object::draw(sf::RenderWindow& window, ResourceManager& resources) {
    if (Constants::DEBUG_MODE) {
        velocity_line[0].position = position;
        velocity_line[0].color = sf::Color::Green;
        velocity_line[1].position = position + velocity * 1.f;
        velocity_line[1].color = sf::Color::Green;

        acceleration_line[0].position = position;
        acceleration_line[0].color = sf::Color::Red;
        acceleration_line[1].position = position + acceleration * 1.f;
        acceleration_line[1].color = sf::Color::Red;

        window.draw(velocity_line);
        window.draw(acceleration_line);
    }
}

void Object::updateVelocity(EngineState& state, float delta) {
    acceleration = getForces(state) * getMassInvert();
    velocity += acceleration * delta;
}

void Object::updatePosition(EngineState& state, float delta) {
    position += velocity * delta;
}

bool Object::detectCollision(Object& obj, CollisionData& data) {
    // si les objets ne sont pas sur la même couche,
    // ils ne peuvent pas entrer en collision
    if (getLayer() != obj.getLayer()) {
        return false;
    }

    // si les deux boîtes englobantes des deux objets ne
    // s'intersectent pas, il ne risque pas d'y avoir de collision
    if (!getAABB()->intersects(*obj.getAABB())) {
        return false;
    }

    return Collision::dispatch[
        // la fonction de détection fine de collision est choisie
        // en fonction des types des deux objets en question
        std::make_pair(getTypeId(), obj.getTypeId())
    ](data);
}

void Object::solveCollision(Object& obj, sf::Vector2f normal) {
    // si les deux objets sont de masse infinie, réinitialisation
    // des vitesses en tant que collision
    if (getMassInvert() == 0 && obj.getMassInvert() == 0) {
        setVelocity(sf::Vector2f(0, 0));
        obj.setVelocity(sf::Vector2f(0, 0));
        return;
    }

    sf::Vector2f rel_velo = obj.getVelocity() - getVelocity();
    float dot_normal = rel_velo.x * normal.x + rel_velo.y * normal.y;

    // si les directions sont divergentes, pas besoin
    // de résoudre la collision
    if (dot_normal >= 0) {
        return;
    }

    // calcule et applique l'impulsion de résolution de la collision
    float restitution = std::min(getRestitution(), obj.getRestitution());
    float collision_impulse = (-(1 + restitution) * dot_normal) /
        (getMassInvert() + obj.getMassInvert());

    setVelocity(getVelocity() - getMassInvert() * collision_impulse * normal);
    obj.setVelocity(obj.getVelocity() + obj.getMassInvert() * collision_impulse * normal);

    // application des forces de frottement entre les deux objets
    // on calcule le vecteur tangent qui porte la force de frottement.
    // les coefficients de friction utilisés sont les moyennes de ceux des deux objets
    rel_velo = obj.getVelocity() - getVelocity();
    dot_normal = rel_velo.x * normal.x + rel_velo.y * normal.y;

    sf::Vector2f tangent = rel_velo - dot_normal * normal;
    float tangent_length = std::sqrt(tangent.x * tangent.x + tangent.y * tangent.y);

    // la tangente est nulle : pas de frottement à générer
    // on évite ainsi une division par zéro
    if (tangent_length == 0) {
        return;
    }

    tangent /= tangent_length;

    float magnitude = -(rel_velo.x * tangent.x + rel_velo.y * tangent.y) /
        (getMassInvert() + obj.getMassInvert());
    float static_friction = (getStaticFriction() + obj.getStaticFriction()) / 2.f;
    float dynamic_friction = (getDynamicFriction() + obj.getDynamicFriction()) / 2.f;
    float friction_impulse;

    // utilisation de la loi de Coulomb sur les frottements dynamiques/statiques
    // cf https://fr.wikipedia.org/wiki/Loi_de_Coulomb_(m%C3%A9canique)
    if (std::abs(magnitude) < collision_impulse * static_friction) {
        friction_impulse = magnitude;
    } else {
        friction_impulse = -collision_impulse * dynamic_friction;
    }

    setVelocity(getVelocity() - getMassInvert() * friction_impulse * tangent);
    obj.setVelocity(obj.getVelocity() + obj.getMassInvert() * friction_impulse * tangent);
}

void Object::positionalCorrection(Object& obj, sf::Vector2f normal, float depth) {
    // ne pas corriger les petites erreurs de position
    // pour éviter l'instabilité du moteur
    if (depth <= Constants::CORRECTION_THRESHOLD) {
        return;
    }

    float position_correction = depth / (getMassInvert() + obj.getMassInvert()) *
        Constants::CORRECTION_PERCENTAGE;

    setPosition(getPosition() - getMassInvert() * position_correction * normal);
    obj.setPosition(obj.getPosition() + obj.getMassInvert() * position_correction * normal);
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

float Object::getStaticFriction() {
    return static_friction;
}

void Object::setStaticFriction(float set_static_friction) {
    static_friction = set_static_friction;
}

float Object::getDynamicFriction() {
    return dynamic_friction;
}

void Object::setDynamicFriction(float set_dynamic_friction) {
    dynamic_friction = set_dynamic_friction;
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
