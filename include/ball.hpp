#ifndef __PTF_BALL_HPP__
#define __PTF_BALL_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine_state.hpp"
#include "object.hpp"

class Ball : public Object {
private:
    sf::Texture texture;
    sf::CircleShape shape;

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
    virtual void draw(sf::RenderWindow& window);

    /**
     * Récupère la boîte englobante de l'objet
     */
    std::unique_ptr<sf::FloatRect> getAABB();

    /**
     * Calcule la normale de la collision de cet objet avec un autre
     */
    virtual bool getNormal(Object& obj, sf::Vector2f& normal);
    virtual bool getNormal(Ball& obj, sf::Vector2f& normal);
    virtual bool getNormal(Block& obj, sf::Vector2f& normal);
};

#endif
