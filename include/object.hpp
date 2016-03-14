#ifndef __PTF_OBJECT_HPP__
#define __PTF_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "engine_state.hpp"

class Object {
protected:
    sf::Vector2f position;
    float charge;
    int layer;

public:
    Object(float x, float y);

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window) = 0;

    /**
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    virtual void update(EngineState& state) = 0;

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() = 0;

    /**
     * Récupère la position de l'objet
     */
    sf::Vector2f getPosition();

    /**
     * Récupère la couche d'affichage de l'objet
     */
    unsigned int getLayer();

    /**
     * Modifie la couche d'affichage de l'objet
     */
    void setLayer(unsigned int set_layer);

    /**
     * Récupère la charge de l'objet
     */
    float getCharge();

    /**
     * Modifie la charge de l'objet
     */
    void setCharge(float set_charge);
};

/**
 * Classe de comparaison des couches deux objets
 * Renvoie "true" si le premier objet est sur une couche
 * qui doit être dessinée avant celle du second
 */
struct ObjectCompare {
	bool operator()(Object* const &t1, Object* const &t2);
};

#endif
