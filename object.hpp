#ifndef PTF_OBJECT_HPP
#define PTF_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Object {
protected:
    sf::Vector2f position;
    int charge;

public:
    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window) = 0;

    /**
     * Récupère la position de l'objet
     */
    sf::Vector2f getPosition();

    /**
     * Récupère la charge de l'objet
     */
    int getCharge();
};

#endif
