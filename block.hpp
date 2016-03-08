#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "state.hpp"

class Block : public Object {
public:
    Block();

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
