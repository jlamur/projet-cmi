#ifndef __SKIZZLE_SWITCH_BLOCK_HPP__
#define __SKIZZLE_SWITCH_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "block.hpp"

class Game;

/**
 * Le bloc changeur est un bloc spécial qui inverse la polarité
 * des joueurs qui passent dessus. Ce bloc ne peut être utilisé
 * qu'une seule fois par partie
 */
class SwitchBlock : public Block {
public:
    typedef std::shared_ptr<SwitchBlock> Ptr;

    /**
     * Identifiant unique du type "bloc changeur"
     */
    static const unsigned int TYPE_ID;

    SwitchBlock();
    virtual ~SwitchBlock();

    /**
     * Clone ce bloc changeur en un bloc changeur avec les mêmes propriétés
     */
    virtual Object::Ptr clone() const;

    /**
     * Dessine le bloc
     */
    virtual void draw(Level& level);

    /**
     * Appelé lorsque le bloc changeur est activé par un objet
     */
    virtual void activate(Game& game, Object::Ptr object);

    /**
     * Récupère l'identifiant de type des blocs changeurs
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement d'un bloc changeur depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le bloc changeur dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;

protected:
    /**
     * Initialisation des propriétés du bloc changeur donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

private:
    sf::Sprite icon_sprite;
    float opacity;
    bool used;
};

#endif
