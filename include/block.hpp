#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "engine_state.hpp"

class Block : public Object {
private:
    sf::Sprite sprite;

public:
    Block(float x, float y);

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window, ResourceManager& resources);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Calcule les informations sur une éventuelle collision de
     * cet objet avec un autre : la normale et la profondeur
     */
    virtual bool getCollisionInfo(Object& obj, sf::Vector2f& normal, float& depth);
    virtual bool getCollisionInfo(Player& obj, sf::Vector2f& normal, float& depth);
    virtual bool getCollisionInfo(Block& obj, sf::Vector2f& normal, float& depth);
};

#endif

