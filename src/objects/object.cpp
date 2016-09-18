#include <cmath>
#include "manager.hpp"
#include "level_data.hpp"
#include "states/game.hpp"
#include "objects/geometry.hpp"
#include "objects/object.hpp"

// propriété signalant la fin d'un objet
const unsigned int PROP_END = 0;

// identifiant de la propriété de masse et sa valeur par défaut
// (une masse de zéro représente une masse infinie)
const unsigned int PROP_MASS = 1;
const float DEFAULT_MASS = 0.f;

// identifiant de la propriété de charge et sa valeur par défaut
const unsigned int PROP_CHARGE = 2;
const float DEFAULT_CHARGE = 0.f;

// identifiant de la propriété de restitution et sa valeur par défaut
// (plus la restitution est forte, plus les objets rebondissent)
const unsigned int PROP_RESTITUTION = 3;
const float DEFAULT_RESTITUTION = 0.4f;

// identifiant du coefficient de frottement statique et sa valeur par défaut
// (coefficient proportionnel à la qté d'énergie nécessaire pour mettre
// en mouvement l'objet)
const unsigned int PROP_STATIC_FRICTION = 4;
const float DEFAULT_STATIC_FRICTION = 0.4f;

// identifiant du coefficient de frottement dynamique et sa valeur par défaut
// (coefficient proportionnel aux pertes d'énergie en mouvement)
const unsigned int PROP_DYNAMIC_FRICTION = 5;
const float DEFAULT_DYNAMIC_FRICTION = 0.2f;

// identifiant de la propriété calque et sa valeur par défaut
// (les objets sur deux calques différents n'entrent pas en collision,
// et les objets sont dessinés par ordre de calque)
const unsigned int PROP_LAYER = 6;
const int DEFAULT_LAYER = 0;

// coefficient d'attraction. Proportionnel à la quantité d'énergie
// fournie par un objet chargé
const float ATTRACTION = 500000;

// coefficients de correction positionnelle permettant de réduire
// la visibilité des erreurs d'arrondi des flottants. Le pourcentage
// de correction indique la proportion de correction par rapport à la
// vitesse et le seuil indique le minimum de correction appliqué
const float CORRECTION_PERCENTAGE = .5f;
const float CORRECTION_SLOP = .02f;

Object::Object(sf::Vector2f position) :
    position(position), selected(false),
    mass(DEFAULT_MASS), inv_mass(1 / DEFAULT_MASS),
    charge(DEFAULT_CHARGE),
    restitution(DEFAULT_RESTITUTION),
    static_friction(DEFAULT_STATIC_FRICTION),
    dynamic_friction(DEFAULT_DYNAMIC_FRICTION),
    layer(DEFAULT_LAYER) {}

void Object::read(std::ifstream& file) {
    // lecture de la position de l'objet
    float pos_x, pos_y;

    file.read(reinterpret_cast<char*>(&pos_x), 4);
    file.read(reinterpret_cast<char*>(&pos_y), 4);

    position.x = pos_x * LevelData::GRID;
    position.y = pos_y * LevelData::GRID;

    // lecture des propriétés de l'objet
    char prop_type = -1;

    while (file.read(&prop_type, 1)) {
        if (prop_type == 0) {
            break;
        }

        if (!readProperty(prop_type, file)) {
            throw std::runtime_error(
                "Impossible de lire l'objet. La propriété de type \"" +
                std::to_string(prop_type) + "\" n'a pas été reconnue"
            );
        }
    }
}

void Object::save(std::ofstream& file) const {
    // écriture de la position de l'objet
    float pos_x = getPosition().x / LevelData::GRID;
    float pos_y = getPosition().y / LevelData::GRID;

    file.write(reinterpret_cast<const char*>(&pos_x), 4);
    file.write(reinterpret_cast<const char*>(&pos_y), 4);

    // écriture des propriétés de l'objet
    writeProperties(file);

    // on termine par un octet nul pour signaler la fin
    char null_byte = 0;
    file.write(&null_byte, 1);
}

bool Object::readProperty(unsigned int prop_type, std::ifstream& file) {
    switch (prop_type) {
    case PROP_MASS:
        float mass;
        file.read(reinterpret_cast<char*>(&mass), 4);
        setMass(mass);
        return true;

    case PROP_CHARGE:
        float charge;
        file.read(reinterpret_cast<char*>(&charge), 4);
        setCharge(charge);
        return true;

    case PROP_RESTITUTION:
        float restitution;
        file.read(reinterpret_cast<char*>(&restitution), 4);
        setRestitution(restitution);
        return true;

    case PROP_STATIC_FRICTION:
        float static_friction;
        file.read(reinterpret_cast<char*>(&static_friction), 4);
        setStaticFriction(static_friction);
        return true;

    case PROP_DYNAMIC_FRICTION:
        float dynamic_friction;
        file.read(reinterpret_cast<char*>(&dynamic_friction), 4);
        setDynamicFriction(dynamic_friction);
        return true;

    case PROP_LAYER:
        char layer;
        file.read(&layer, 1);
        setLayer((int) layer - 127);
        return true;

    default:
        return false;
    }
}

void Object::writeProperties(std::ofstream& file) const {
    char prop_type;

    if (mass != DEFAULT_MASS) {
        prop_type = PROP_MASS;
        file.write(&prop_type, 1);
        file.write(reinterpret_cast<const char*>(&mass), 4);
    }

    if (charge != DEFAULT_CHARGE) {
        prop_type = PROP_CHARGE;
        file.write(&prop_type, 1);
        file.write(reinterpret_cast<const char*>(&charge), 4);
    }

    if (restitution != DEFAULT_RESTITUTION) {
        prop_type = PROP_RESTITUTION;
        file.write(&prop_type, 1);
        file.write(reinterpret_cast<const char*>(&restitution), 4);
    }

    if (static_friction != DEFAULT_STATIC_FRICTION) {
        prop_type = PROP_STATIC_FRICTION;
        file.write(&prop_type, 1);
        file.write(reinterpret_cast<const char*>(&static_friction), 4);
    }

    if (dynamic_friction != DEFAULT_DYNAMIC_FRICTION) {
        prop_type = PROP_DYNAMIC_FRICTION;
        file.write(&prop_type, 1);
        file.write(reinterpret_cast<const char*>(&dynamic_friction), 4);
    }

    if (layer != DEFAULT_LAYER) {
        prop_type = PROP_LAYER;
        file.write(&prop_type, 1);

        char write_layer = layer + 127;
        file.write(&write_layer, 1);
    }
}

void Object::updateVelocity(const Game& game) {
    acceleration = getForces(game) * getMassInvert();
    velocity += acceleration * Manager::FRAME_TIME.asSeconds();
}

void Object::updatePosition() {
    position += velocity * Manager::FRAME_TIME.asSeconds();
}

void Object::solveCollision(Game& game, Object& obj, const sf::Vector2f& normal) {
    // si les deux objets sont de masse infinie, réinitialisation
    // des vitesses en tant que collision
    if (getMassInvert() == 0 && obj.getMassInvert() == 0) {
        velocity = sf::Vector2f(0, 0);
        obj.velocity = sf::Vector2f(0, 0);
        return;
    }

    sf::Vector2f rel_velo = obj.getVelocity() - getVelocity();
    float dot_normal = rel_velo.x * normal.x + rel_velo.y * normal.y;

    // en ce point, on est bertins qu'une collision a eu lieu.
    // on peut donc activer les deux objets
    activate(game, obj);
    obj.activate(game, *this);

    // si les directions sont divergentes, pas besoin
    // de résoudre la collision
    if (dot_normal > 0) {
        return;
    }

    // on utilise le plus petit coefficient de friction entre les
    // deux objets comme le coefficient de la collision
    float restitution = std::min(getRestitution(), obj.getRestitution());

    // calcule et applique l'impulsion de résolution de la collision
    float collision_impulse = -(1.f + restitution) * std::min(dot_normal + .8f, 0.f) /
        (getMassInvert() + obj.getMassInvert());

    velocity -= getMassInvert() * collision_impulse * normal;
    obj.velocity += obj.getMassInvert() * collision_impulse * normal;

    // application des forces de frottement entre les deux objets
    // on calcule le vecteur tangent qui porte la force de frottement.
    // les coefficients de friction utilisés sont les moyennes de ceux des deux objets
    rel_velo = obj.getVelocity() - getVelocity();
    dot_normal = rel_velo.x * normal.x + rel_velo.y * normal.y;

    sf::Vector2f tangent = rel_velo - dot_normal * normal;
    float tangent_length = tangent.x * tangent.x + tangent.y * tangent.y;

    // la tangente est nulle : pas de frottement à générer
    // on évite ainsi une division par zéro
    if (tangent_length == 0) {
        return;
    }

    tangent /= std::sqrt(tangent_length);

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

    velocity -= getMassInvert() * friction_impulse * tangent;
    obj.velocity += obj.getMassInvert() * friction_impulse * tangent;
}

void Object::positionalCorrection(Object& obj, const sf::Vector2f& normal, float depth) {
    float position_correction = std::max(depth - CORRECTION_SLOP, 0.0f) /
        (getMassInvert() + obj.getMassInvert()) * CORRECTION_PERCENTAGE;

    position -= getMassInvert() * position_correction * normal;
    obj.position += obj.getMassInvert() * position_correction * normal;
}

sf::Vector2f Object::getForces(const Game& game) const {
    sf::Vector2f forces(0, 0);
    const LevelData::ObjectList& objects = game.getLevel().getObjects();

    // force de gravité
    forces += getMass() * game.getGravity();

    // force d'attraction entre objets chargés
    if (getCharge() != 0) {
        for (unsigned int j = 0; j < objects.size(); j++) {
            Object& attractor = *objects[j];

            if (this == objects[j].get() || attractor.getCharge() == 0) {
                continue;
            }

            // vecteur allant de l'objet attracteur vers l'objet actuel
            sf::Vector2f attraction(getPosition() - attractor.getPosition());

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
            attraction *= ATTRACTION * (
                (getCharge() * attractor.getCharge()) /
                distance_squared
            );

            forces += attraction;
        }
    }

    return forces;
}

sf::Vector2f Object::getAcceleration() const {
    return acceleration;
}

sf::Vector2f Object::getVelocity() const {
    return velocity;
}

sf::Vector2f Object::getPosition() const {
    return position;
}

bool Object::isSelected() const {
    return selected;
}

void Object::setSelected(bool selected) {
    this->selected = selected;
}

float Object::getMass() const {
    return mass;
}

float Object::getMassInvert() const {
    return inv_mass;
}

void Object::setMass(float mass) {
    this->mass = mass;

    if (mass == 0) {
        inv_mass = 0;
    } else {
        inv_mass = 1 / mass;
    }
}

float Object::getCharge() const {
    return charge;
}

void Object::setCharge(float charge) {
    this->charge = charge;
}

float Object::getRestitution() const {
    return restitution;
}

void Object::setRestitution(float restitution) {
    this->restitution = restitution;
}

float Object::getStaticFriction() const {
    return static_friction;
}

void Object::setStaticFriction(float static_friction) {
    this->static_friction = static_friction;
}

float Object::getDynamicFriction() const {
    return dynamic_friction;
}

void Object::setDynamicFriction(float dynamic_friction) {
    this->dynamic_friction = dynamic_friction;
}

int Object::getLayer() const {
    return layer;
}

void Object::setLayer(int layer) {
    this->layer = layer;
}

bool ObjectCompare::operator()(const Object& t1, const Object& t2) const {
    sf::Vector2f t1_pos = t1.getPosition();
    sf::Vector2f t2_pos = t2.getPosition();

    return t1_pos.x - t1_pos.y + t1.getLayer() >
           t2_pos.x - t2_pos.y + t2.getLayer();
}
