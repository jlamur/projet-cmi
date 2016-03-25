#ifndef __PTF_BALL_HPP__
#define __PTF_BALL_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "engine_state.hpp"

class Ball : public Object {
private:
    sf::Sprite sprite;

protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(EngineState& state);

public:
    Ball(float x, float y);

    /**
     * Dessine la balle dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window, ResourceManager& resources);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Récupère l'identifiant de type de cet objet
     */
    static constexpr unsigned int TYPE_ID = 0;
    unsigned int getTypeId();

    /**
     * Renvoie le rayon de la balle
     */
    float getRadius();
};

#endif
