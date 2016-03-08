#ifndef PTF_BLOCK_HPP
#define PTF_BLOCK_HPP

#include "object.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Block : public Object {
public:
    Block();

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);
};

#endif
