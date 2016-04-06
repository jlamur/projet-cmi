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
    /**
     * Identifiant unique du type d'objet
     */
    static const unsigned int TYPE_ID;

    Player();
    virtual ~Player();

    /**
     * Clone cet objet en un objet avec les mêmes propriétés
     */
    virtual ObjectPtr clone() const;

    /**
     * Chargement de l'objet depuis le fichier donné
     */
    static ObjectPtr load(std::ifstream& file);

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    virtual void draw(Manager& manager);

    /**
     * Met à jour la position de l'objet selon sa
     * vitesse actuelle
     */
    virtual void updatePosition(float delta);

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() const;

    /**
     * Récupère l'identifiant de type de cet objet
     */
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
