#ifndef __PTF_PHYSICS_OBJECT_HPP__
#define __PTF_PHYSICS_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <iostream>
#include "object.hpp"
#include "engine_state.hpp"

class PhysicsObject : public Object {
protected:
    float mass;
    sf::Vector2f acceleration;
    sf::VertexArray accelLine;
    sf::Vector2f velocity;
    sf::VertexArray velLine;

    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(EngineState& state);

    static constexpr float GRAVITY = 20;

public:
    PhysicsObject(float x, float y) :
        Object(x, y), mass(1), accelLine(sf::LinesStrip, 2),
        velLine(sf::LinesStrip, 2) {}

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window);

    /**
     * Met à jour la physique de l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    void update(EngineState& state);

    /**
     * Récupère la vitesse de l'objet
     */
    sf::Vector2f getVelocity();

    /**
     * Modifie la vitesse de l'objet
     * (à utiliser avec précaution, préférer modifier les forces)
     */
    void setVelocity(sf::Vector2f set_velocity);

    /**
     * Récupère la masse de l'objet
     */
    int getMass();

    /**
     * Modifie la masse de l'objet
     */
    void setMass(int set_mass);
};

#endif
