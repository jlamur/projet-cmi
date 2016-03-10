#ifndef __PTF_BALL_HPP__
#define __PTF_BALL_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.hpp"
#include "physics_object.hpp"

class Ball : public PhysicsObject {
protected:
    sf::CircleShape shape;

    /**
     * Calcule les forces appliquées à l'objet
     */
    sf::Vector2f getForces(State state);

    static constexpr float ATTRACTION = 10;
    static constexpr float MOVE = 10;

public:
    Ball(float x, float y) : PhysicsObject(x, y), shape(10 * mass) {
        shape.setFillColor(sf::Color(150,255,20));
    }

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
