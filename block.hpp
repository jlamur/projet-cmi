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
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état du moteur et le temps écoulé depuis la dernière frame
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
