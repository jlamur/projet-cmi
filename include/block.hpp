#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "engine_state.hpp"

class Block : public Object {
private:
    sf::Texture texture;
    sf::RectangleShape shape;

public:
    Block(float x, float y);

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Calcule la normale de la collision de cet objet avec un autre
     */
    virtual bool getNormal(Object& obj, sf::Vector2f& normal);
    virtual bool getNormal(Ball& obj, sf::Vector2f& normal);
    virtual bool getNormal(Block& obj, sf::Vector2f& normal);
};

#endif
