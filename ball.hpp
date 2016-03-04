#ifndef PTF_BALL_HPP
#define PTF_BALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Ball {
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float mass;
    int charge;

public:
    Ball();

    void update(float delta);
    void draw(sf::RenderWindow& window);

    // getters et setters
    sf::Vector2f getPosition() {
        return position;
    }

    sf::Vector2f getVelocity() {
        return velocity;
    }

    float getMass() {
        return mass;
    }

    int getCharge() {
        return charge;
    }
};

#endif
