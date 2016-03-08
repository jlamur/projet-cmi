#ifndef PTF_BALL_HPP
#define PTF_BALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.hpp"
#include "physics_object.hpp"

class Ball : public PhysicsObject {
protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    void getForces(State state);

public:
    Ball();

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);

    /**
     * Détermine la couche d'affichage de l'objet
     */
    unsigned int getLayer() {
        return 1;
    }
};

#endif
