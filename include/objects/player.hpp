#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "object.hpp"

/**
 * Le joueur est un objet du jeu qui peut être déplacé au clavier
 * et interagit avec les autres objets. Il possède un statut particulier
 * au sein du niveau car il détermine si l'on gagne ou on perd la partie
 */
class Player : public Object {
public:
    typedef std::shared_ptr<Player> Ptr;

private:
    mutable sf::CircleShape sprite;
    sf::CircleShape shadow_sprite;

    mutable sf::Vector2f previous_position;
    unsigned int player_number;

protected:
    /**
     * Calcule les forces appliquées au joueur
     */
    virtual sf::Vector2f getForces(const Game& game) const;

    /**
     * Initialisation des propriétés du joueur donné
     * depuis le fichier donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

public:
    /**
     * Identifiant unique du type "joueur"
     */
    static const unsigned int TYPE_ID;

    Player();
    virtual ~Player();

    /**
     * Clone ce joueur en un joueur avec les mêmes propriétés
     */
    virtual Object::Ptr clone() const;

    /**
     * Récupère la boîte englobante du joueur
     */
    virtual sf::FloatRect getAABB() const;

    /**
     * Récupère le rayon du joueur
     */
    virtual float getRadius() const;

    /**
     * Dessine le joueur dans la fenêtre donnée
     */
    virtual void draw(Level& manager);

    /**
     * Appelé lorsque le joueur est activé par un objet
     */
    virtual void activate(Game& game, Object::Ptr object);

    /**
     * Récupère l'identifiant de type des joueurs
     */
    virtual unsigned int getTypeId() const;

    /**
     * Récupère le type de collision des joueurs
     */
    virtual CollisionType getCollisionType() const;

    /**
     * Chargement d'un joueur depuis le fichier donné
     */
    static Object::Ptr load(std::ifstream& file);

    /**
     * Sauvegarde le joueur dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;

    /**
     * Renvoie le numéro du joueur
     */
    unsigned int getPlayerNumber() const;

    /**
     * Modifie le numéro du joueur
     */
    void setPlayerNumber(unsigned int set_number);
};
