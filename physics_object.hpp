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
    sf::Vector2f getForces(State state);

    static constexpr float GRAVITY = 10;

public:
    /**
     * Met à jour la physique de l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    void update(State state);
};

#endif
