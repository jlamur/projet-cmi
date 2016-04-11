#ifndef __PTF_SWITCH_BLOCK_HPP__
#define __PTF_SWITCH_BLOCK_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "block.hpp"
#include "level.hpp"

class SwitchBlock : public Block {
public:
    typedef std::shared_ptr<SwitchBlock> Ptr;

protected:
    /**
     * Initialisation des propriétés du bloc changeur donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

public:
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
     * Prépare les textures avant le dessin du bloc
     */
    virtual void prepareDraw(ResourceManager& resources);

    /**
     * Appelé lorsque le bloc changeur est activé par un objet
     */
    virtual void activate(Level& level, Object* object);

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
};

#endif
