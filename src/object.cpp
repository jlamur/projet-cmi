#include "object.hpp"
#include "constants.hpp"
#include "collision.hpp"

const unsigned int Object::PROP_MASS = 1;
const unsigned int Object::PROP_CHARGE = 2;
const unsigned int Object::PROP_RESTITUTION = 3;
const unsigned int Object::PROP_STATIC_FRICTION = 4;
const unsigned int Object::PROP_DYNAMIC_FRICTION = 5;
const unsigned int Object::PROP_LAYER = 6;

Object::Object() :
    acceleration(0, 0), velocity(0, 0), position(0, 0),
    acceleration_line(sf::Lines, 2),
    velocity_line(sf::Lines, 2),
    inv_mass(-1.f),

    // valeurs par défaut pour les propriétés
    // de tous les objets du jeu
    mass(1.f), charge(0.f),
    restitution(0.4f),
    static_friction(0.4f),
    dynamic_friction(0.2f),
    layer(0) {}

Object::~Object() {}

void Object::load(std::ifstream& file, std::shared_ptr<Object> object) {
    // lecture de la position de l'objet
    float pos_x, pos_y;

    file.read(reinterpret_cast<char*>(&pos_x), sizeof(pos_x));
    file.read(reinterpret_cast<char*>(&pos_y), sizeof(pos_y));

    object->setPosition(sf::Vector2f(
        pos_x * Constants::GRID, pos_y * Constants::GRID
    ));

    // lecture des propriétés facultatives
    char prop_type = -1;

    while (file.read(&prop_type, 1)) {
        switch (prop_type) {
        case Object::PROP_MASS:
            float mass;
            file.read(reinterpret_cast<char*>(&mass), sizeof(mass));
            object->setMass(mass);
            break;

        case Object::PROP_CHARGE:
            float charge;
            file.read(reinterpret_cast<char*>(&charge), sizeof(charge));
            object->setCharge(charge);
            break;

        case Object::PROP_RESTITUTION:
            float restitution;
            file.read(reinterpret_cast<char*>(&restitution), sizeof(restitution));
            object->setRestitution(restitution);
            break;

        case Object::PROP_STATIC_FRICTION:
            float static_friction;
            file.read(reinterpret_cast<char*>(&static_friction), sizeof(static_friction));
            object->setStaticFriction(static_friction);
            break;

        case Object::PROP_DYNAMIC_FRICTION:
            float dynamic_friction;
            file.read(reinterpret_cast<char*>(&dynamic_friction), sizeof(dynamic_friction));
            object->setDynamicFriction(dynamic_friction);
            break;

        case Object::PROP_LAYER:
            char layer;
            file.read(&layer, 1);
            object->setLayer((int) layer - 127);
            break;

        default:
            // propriété de type inconnu : on recule
            // d'un octet et on sort
            file.seekg(-1, file.cur);
            return;
        }
    }
}

sf::Vector2f Object::getForces(
    const Manager& manager, const std::vector<ObjectPtr>& objects
) const {
    sf::Vector2f forces(0, 0);

    // force de gravité
    forces += sf::Vector2f(0, getMass() * Constants::GRAVITY);

    // force d'attraction entre objets chargés
    if (getCharge() != 0) {
        for (unsigned int j = 0; j < objects.size(); j++) {
            ObjectPtr attractive = objects[j];

            if (attractive.get() == this || attractive->getCharge() == 0) {
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

void Object::draw(Manager& manager) {
    if (Constants::DEBUG_MODE) {
        velocity_line[0].position = position;
        velocity_line[0].color = sf::Color::Green;
        velocity_line[1].position = position + velocity * 1.f;
        velocity_line[1].color = sf::Color::Green;

        acceleration_line[0].position = position;
        acceleration_line[0].color = sf::Color::Red;
        acceleration_line[1].position = position + acceleration * 1.f;
        acceleration_line[1].color = sf::Color::Red;

        manager.getWindow().draw(velocity_line);
        manager.getWindow().draw(acceleration_line);
    }
}

void Object::updateVelocity(
    const Manager& manager, const std::vector<ObjectPtr>& objects, float delta
) {
    acceleration = getForces(manager, objects) * getMassInvert();
    velocity += acceleration * delta;
}

void Object::updatePosition(float delta) {
    position += velocity * delta;
}

bool Object::detectCollision(const Object& obj, CollisionData& data) const {
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

    return getCollisionData(data);
}

void Object::solveCollision(Object& obj, const sf::Vector2f& normal) {
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

void Object::positionalCorrection(Object& obj, const sf::Vector2f& normal, float depth) {
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

sf::Vector2f Object::getAcceleration() const {
    return acceleration;
}

sf::Vector2f Object::getVelocity() const {
    return velocity;
}

void Object::setVelocity(sf::Vector2f set_velocity) {
    velocity = set_velocity;
}

sf::Vector2f Object::getPosition() const {
    return position;
}

void Object::setPosition(sf::Vector2f set_position) {
    position = set_position;
}

float Object::getMass() const {
    return mass;
}

float Object::getMassInvert() const {
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

float Object::getCharge() const {
    return charge;
}

void Object::setCharge(float set_charge) {
    charge = set_charge;
}

float Object::getRestitution() const {
    return restitution;
}

void Object::setRestitution(float set_restitution) {
    restitution = set_restitution;
}

float Object::getStaticFriction() const {
    return static_friction;
}

void Object::setStaticFriction(float set_static_friction) {
    static_friction = set_static_friction;
}

float Object::getDynamicFriction() const {
    return dynamic_friction;
}

void Object::setDynamicFriction(float set_dynamic_friction) {
    dynamic_friction = set_dynamic_friction;
}

int Object::getLayer() const {
    return layer;
}

void Object::setLayer(int set_layer) {
    layer = set_layer;
}

bool ObjectCompare::operator()(ObjectPtr const &t1, ObjectPtr const &t2) const {
    return t1->getLayer() > t2->getLayer();
}
