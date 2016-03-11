#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "state.hpp"

class Block : public Object {
protected:
    sf::RectangleShape shape;

public:
    static constexpr float GRID = 32;

    Block(float x, float y) : Object(x, y), shape(sf::Vector2f(Block::GRID, Block::GRID)) {
        shape.setOrigin(sf::Vector2f(Block::GRID / 2, Block::GRID / 2));
        shape.setFillColor(sf::Color(0, 0, 0));
    }

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
