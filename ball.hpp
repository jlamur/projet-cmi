#ifndef __PTF_BALL_HPP__
#define __PTF_BALL_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.hpp"
#include "physics_object.hpp"

class Ball : public PhysicsObject {
protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    sf::Vector2f getForces(State state);

    static constexpr float ATTRACTION = 10;
    static constexpr float MOVE = 10;

public:
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
