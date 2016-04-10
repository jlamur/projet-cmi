#ifndef __PTF_BLOCK_HPP__
#define __PTF_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "object.hpp"

class Block : public Object {
public:
    typedef std::shared_ptr<Block> Ptr;

private:
    sf::Sprite select_sprite;

protected:
    mutable sf::Sprite sprite;

    /**
     * Initialisation des propriétés du bloc donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

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
    virtual Object::Ptr clone() const;

    /**
     * Récupère la boîte englobante du bloc
     */
    virtual sf::FloatRect getAABB() const;

    /**
     * Récupère le rayon du bloc
     */
    virtual float getRadius() const;

    /**
     * Calcule la texture à afficher pour ce bloc
     */
    virtual std::string getTexture();

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(Level& level);

    /**
     * Appelé lorsque le bloc est activé par un objet
     */
    virtual void activated(Level& level, Object* object);

    /**
     * Récupère l'identifiant de type des blocs
     */
    virtual unsigned int getTypeId() const;

    /**
     * Récupère le type de collision des blocs
     */
    virtual CollisionType getCollisionType() const;

    /**
     * Chargement d'un bloc depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le bloc dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;
};

#endif
