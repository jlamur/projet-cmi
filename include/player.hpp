#ifndef __PTF_PLAYER_HPP__
#define __PTF_PLAYER_HPP__

#include <SFML/Graphics.hpp>
#include "object.hpp"

class Player : public Object {
private:
    mutable sf::Sprite sprite;
    unsigned int player_number;

protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(const Manager& manager, const std::vector<ObjectPtr>& objects) const;

public:
    Player(float x, float y);
    virtual ~Player();

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    virtual void draw(Manager& manager);

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() const;

    /**
     * Récupère l'identifiant de type de cet objet
     */
    static const unsigned int TYPE_ID;
    virtual unsigned int getTypeId() const;

    /**
     * Renvoie le rayon de la balle
     */
    float getRadius() const;

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
