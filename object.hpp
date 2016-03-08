#ifndef PTF_OBJECT_HPP
#define PTF_OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "state.hpp"

class Object {
protected:
    sf::Vector2f position;
    int charge;

public:
    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window) = 0;

    /**
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état du moteur et le temps écoulé depuis la dernière frame
     */
    virtual void update(State state) = 0;

    /**
     * Détermine la couche d'affichage de l'objet
     */
    virtual unsigned int getLayer() = 0;

    /**
     * Récupère la position de l'objet
     */
    sf::Vector2f getPosition();

    /**
     * Récupère la charge de l'objet
     */
    int getCharge();
};

/**
 * Classe de comparaison des couches deux objets
 * Renvoie "true" si le premier objet est sur une couche
 * qui doit être dessinée avant celle du second
 */
struct CompareObjectLayer {
    bool operator()(Object* const &obj1, Object* const &obj2) {
        return obj1->getLayer() < obj2->getLayer();
    }
};

#endif
