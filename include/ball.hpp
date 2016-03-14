#ifndef __PTF_BALL_HPP__
#define __PTF_BALL_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine_state.hpp"
#include "physics_object.hpp"

class Ball : public PhysicsObject {
protected:
    sf::Texture texture;
    sf::CircleShape shape;

    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(EngineState& state);

public:
    Ball(float x, float y);

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    void draw(sf::RenderWindow& window);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Détermine la couche d'affichage de l'objet
     */
    unsigned int getLayer();
};

#endif
