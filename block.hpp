#ifndef PTF_BLOCK_HPP
#define PTF_BLOCK_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"

class Block : public Object {
public:
    Block();

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);

    /**
     * Détermine la couche d'affichage de l'objet
     */
    unsigned int getLayer() {
        return 0;
    }
};

#endif
