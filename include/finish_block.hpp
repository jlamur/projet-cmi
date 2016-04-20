#ifndef __SKIZZLE_FINISH_BLOCK_HPP__
#define __SKIZZLE_FINISH_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "block.hpp"

class Game;

/**
 * Le bloc d'arrivée est un bloc spécial qui tue les joueurs
 * qui arrivent dessus et fait gagner le niveau lorsqu'il n'y a
 * plus de joueurs
 */
class FinishBlock : public Block {
public:
    typedef std::shared_ptr<FinishBlock> Ptr;

protected:
    /**
     * Initialisation des propriétés du bloc d'arrivée donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

public:
    /**
     * Identifiant unique du type "bloc d'arrivée"
     */
    static const unsigned int TYPE_ID;

    FinishBlock();
    virtual ~FinishBlock();

    /**
     * Clone ce bloc d'arrivée en un bloc d'arrivée avec les mêmes propriétés
     */
    virtual Object::Ptr clone() const;

    /**
     * Prépare les textures avant le dessin du bloc
     */
    virtual void prepareDraw();

    /**
     * Appelé lorsque le bloc d'arrivée est activé par un objet
     */
    virtual void activate(Game& game, Object::Ptr object);

    /**
     * Récupère l'identifiant de type des blocs d'arrivée
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement d'un bloc d'arrivée depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le bloc d'arrivée dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;
};

#endif
