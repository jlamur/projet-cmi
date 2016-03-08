#ifndef PTF_OBJECT_HPP
#define PTF_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.hpp"
#include "object.hpp"

class PhysicsObject : class Object {
protected:
    sf::Vector2f velocity;
    float mass;

    /**
     * Calcule les forces appliquées à l'objet
     */
    sf::Vector2f getForces(State state);

public:
    /**
     * Met à jour la physique de l'objet juste avant le dessin d'une frame
     * Reçoit l'état du moteur et le temps écoulé depuis la dernière frame
     */
    void update(State state, float delta);
};

#endif
