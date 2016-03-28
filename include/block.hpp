#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include "object.hpp"

class Block : public Object {
private:
    mutable sf::Sprite sprite;

public:
    Block(float x, float y);

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(Manager& manager);

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() const;

    /**
     * Récupère l'identifiant de type de cet objet
     */
    static constexpr unsigned int TYPE_ID = 1;
    virtual unsigned int getTypeId() const;
};

#endif
