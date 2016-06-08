#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "../utility.hpp"
#include "block.hpp"

class Game;

/**
 * Le bloc de gravité est un bloc spécial qui change l'orientation
 * de la gravité dans une direction prédéfinie lorsqu'il est
 * activé par n'importe quel objet. Ce bloc ne peut être utilisé
 * qu'une seule fois par partie
 */
class GravityBlock : public Block {
public:
    typedef std::shared_ptr<GravityBlock> Ptr;

private:
    Utility::Direction gravity_direction;
    sf::Sprite icon_sprite;
    float opacity;
    bool used;

protected:
    /**
     * Initialisation des propriétés du bloc de gravité donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

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
    virtual Object::Ptr clone() const;

    /**
     * Dessin du bloc dans la fenêtre donnée
     */
    virtual void draw(Level& level);

    /**
     * Appelé lorsque le bloc de gravité est activé par un objet
     */
    virtual void activate(Game& game, Object::Ptr object);

    /**
     * Récupère l'identifiant de type des blocs de gravité
     */
    virtual unsigned int getTypeId() const;

    /**
     * Chargement d'un bloc de gravité depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le bloc de gravité dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;

    /**
     * Récupère la direction de gravité du bloc changeur de gravité
     */
    Utility::Direction getGravityDirection() const;

    /**
     * Modifie la direction de gravité du bloc
     */
    void setGravityDirection(Utility::Direction set_gravity_direction);
};
