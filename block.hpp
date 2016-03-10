#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "state.hpp"

class Block : public Object {
protected:
    sf::CircleShape shape;

public:
    Block(float x, float y) : Object(x,y), shape(80,4) {
        shape.setFillColor(sf::Color(0,0,0));
    }

    // utilise le constructeur de Object
    using Object::Object;

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);

    /**
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    void update(State state);

    /**
     * Détermine la couche d'affichage de l'objet
     */
    unsigned int getLayer() {
        return 0;
    }
};

#endif
