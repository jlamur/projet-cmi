#ifndef __PTF_GRAVITY_BLOCK_HPP__
#define __PTF_GRAVITY_BLOCK_HPP__

#include "block.hpp"
#include "level.hpp"

class GravityBlock : public Block {
private:
    GravityDirection gravity_direction;

public:
    /**
     * Identifiant unique du type "bloc de gravité"
     */
    static const unsigned int TYPE_ID;

    GravityBlock();
    virtual ~GravityBlock();

    /**
     * Clone ce bloc de gravité en un bloc de gravité avec les mêmes propriétés
     */
    virtual ObjectPtr clone() const;

    /**
     * Opérations de préparation de la texture du bloc
     */
    virtual void beforeDraw(Level& level);

    /**
     * Appelé lorsque le bloc de gravité est activé par un objet
     */
    virtual void activated(Level& level, Object& object);

    /**
     * Récupère l'identifiant de type des blocs de gravité
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement du bloc de gravité depuis le fichier donné
     */
    static ObjectPtr load(std::ifstream& file);

    /**
     * Récupère la direction de gravité du bloc changeur de gravité
     */
    GravityDirection getGravityDirection() const;

    /**
     * Modifie la direction de gravité du bloc
     */
    void setGravityDirection(GravityDirection set_gravity_direction);
};

#endif
