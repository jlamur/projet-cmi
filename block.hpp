#ifndef PTF_BALL_HPP
#define PTF_BALL_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Block {
protected:
    sf::Vector2i position;
    int charge;

public:
    Block();

    void draw(sf::RenderWindow& window);

    // getters et setters
    sf::Vector2i getPosition() {
        return position;
    }

    int getCharge() {
        return charge;
    }
};

#endif
