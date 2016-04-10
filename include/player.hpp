#ifndef __PTF_PLAYER_HPP__
#define __PTF_PLAYER_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "object.hpp"

class Player : public Object {
public:
    typedef std::shared_ptr<Player> Ptr;

private:
    mutable sf::CircleShape sprite;
    unsigned int player_number;

protected:
    /**
     * Calcule les forces appliquées au joueur
     */
    virtual sf::Vector2f getForces(const Level& level) const;

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
    virtual void activate(Level& level, Object* object);

    /**
     * Appelé lorsque le jouer a été tué. Si cette fonction
     * est appelé, le joueur sera supprimé à la frame suivante
     */
    virtual void kill(Game& game);

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
     * Met à jour la position de l'objet selon sa
     * vitesse actuelle
     */
    virtual void updatePosition();

    /**
     * Renvoie le numéro du joueur
     */
    unsigned int getPlayerNumber() const;

    /**
     * Modifie le numéro du joueur
     */
    void setPlayerNumber(unsigned int set_number);
};

#endif
