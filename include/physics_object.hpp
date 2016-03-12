#ifndef __PTF_PHYSICS_OBJECT_HPP__
#define __PTF_PHYSICS_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "state.hpp"

class PhysicsObject : public Object {
protected:
    sf::Vector2f velocity;
    float mass;

    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(State state);

    static constexpr float GRAVITY = 275;

public:
    PhysicsObject(float x, float y) :
        Object(x, y), mass(1) {}

    /**
     * Met à jour la physique de l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    void update(State state);

    /**
     * Récupère la vitesse de l'objet
     */
    sf::Vector2f getVelocity();

    /**
     * Modifie la vitesse de l'objet
     * (à utiliser avec précaution, préférer modifier les forces)
     */
    void setVelocity(sf::Vector2f set_velocity);

    /**
     * Récupère la masse de l'objet
     */
    int getMass();

    /**
     * Modifie la masse de l'objet
     */
    void setMass(int set_mass);
};

#endif
