#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "engine_state.hpp"

class Block : public Object {
protected:
    sf::Texture texture;
    sf::RectangleShape shape;

public:
    static constexpr float GRID = 32;

    Block(float x, float y) : Object(x, y), shape(sf::Vector2f(Block::GRID, Block::GRID)) {
        shape.setOrigin(sf::Vector2f(Block::GRID / 2, Block::GRID / 2));
    }

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);

    /**
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    void update(EngineState& state);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Détermine la couche d'affichage de l'objet
     */
    unsigned int getLayer() {
        return 0;
    }
};

#endif
