#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
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
     * Récupère l'identifiant de type de cet objet
     */
    static constexpr unsigned int TYPE_ID = 1;
    unsigned int getTypeId();
};

#endif
