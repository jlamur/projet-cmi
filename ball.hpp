#ifndef PTF_BALL_HPP
#define PTF_BALL_HPP

#include "object.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Ball : public Object {
protected:
    sf::Vector2f velocity;
    float mass;

public:
    Ball();

    /**
     * Mise à jour de la position de la balle en fonction des forces
     * qui lui sont appliquées
     */
    void update(sf::Vector2f forces, float delta);

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);
};

#endif
