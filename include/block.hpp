#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include "object.hpp"

class Block : public Object {
private:
    mutable sf::Sprite sprite;
    mutable sf::Sprite select_sprite;

public:
    /**
     * Identifiant unique du type d'objet
     */
    static const unsigned int TYPE_ID;

    Block();
    virtual ~Block();

    /**
     * Clone cet objet en un objet avec les mêmes propriétés
     */
    virtual ObjectPtr clone() const;

    /**
     * Chargement de l'objet depuis le fichier donné
     */
    static ObjectPtr load(std::ifstream& file);

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
    virtual unsigned int getTypeId() const;
};

#endif
