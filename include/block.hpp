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
     * Identifiant unique du type "bloc"
     */
    static const unsigned int TYPE_ID;

    Block();
    virtual ~Block();

    /**
     * Clone ce bloc en un bloc avec les mêmes propriétés
     */
    virtual ObjectPtr clone() const;

    /**
     * Récupère la boîte englobante du bloc
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() const;

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(Manager& manager);

    /**
     * Appelé lorsque le bloc est activé par un objet
     */
    virtual void activated(Object& object);

    /**
     * Récupère l'identifiant de type des blocs
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement du bloc depuis le fichier donné
     */
    static ObjectPtr load(std::ifstream& file);
};

#endif
