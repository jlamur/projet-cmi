#ifndef __SKIZZLE_KILL_BLOCK_HPP__
#define __SKIZZLE_KILL_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "block.hpp"

class Game;

/**
 * Le bloc tueur est un bloc spécial qui tue les joueurs
 * qui arrivent dessus et fait perdre la partie
 */
class KillBlock : public Block {
public:
    typedef std::shared_ptr<KillBlock> Ptr;

    /**
     * Identifiant unique du type "bloc tueur"
     */
    static const unsigned int TYPE_ID;

    KillBlock();
    virtual ~KillBlock();

    /**
     * Clone ce bloc tueur en un bloc tueur avec les mêmes propriétés
     */
    virtual Object::Ptr clone() const;

    /**
     * Dessine le bloc
     */
    virtual void draw(Level& level);

    /**
     * Appelé lorsque le bloc tueur est activé par un objet
     */
    virtual void activate(Game& game, Object::Ptr object);

    /**
     * Récupère l'identifiant de type des blocs tueurs
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement d'un bloc tueur depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le bloc tueur dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;

private:
    sf::Sprite icon_sprite;

protected:
    /**
     * Initialisation des propriétés du bloc tueur donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);
};

#endif
