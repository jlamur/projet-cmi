#ifndef __PTF_OBJECT_HPP__
#define __PTF_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "engine_state.hpp"

class Block;
class Ball;

class Object {
private:
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;

    sf::VertexArray accelerationLine;
    sf::VertexArray velocityLine;

    float mass;
    float charge;
    float restitution;
    int layer;

protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(EngineState& state);

public:
    Object(float x, float y);

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow& window);

    /**
     * Met à jour l'objet juste avant le dessin d'une frame
     * Reçoit l'état actuel du moteur
     */
    virtual void update(EngineState& state);

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() = 0;

    /**
     * Calcule la normale de la collision de cet objet avec un autre
     */
    virtual bool getNormal(Object& obj, sf::Vector2f& normal);
    virtual bool getNormal(Ball& obj, sf::Vector2f& normal) = 0;
    virtual bool getNormal(Block& obj, sf::Vector2f& normal) = 0;

    /**
     * Récupère l'accélération de l'objet
     */
    sf::Vector2f getAcceleration();

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
     * Récupère la position de l'objet
     */
    sf::Vector2f getPosition();

    /**
     * Récupère la masse de l'objet
     */
    float getMass();

    /**
     * Modifie la masse de l'objet
     */
    void setMass(float set_mass);

    /**
     * Récupère la charge de l'objet
     */
    float getCharge();

    /**
     * Modifie la charge de l'objet
     */
    void setCharge(float set_charge);

    /**
     * Récupère le coefficient de restitution de l'objet
     */
    float getRestitution();

    /**
     * Modifie le coefficient de restitution de l'objet
     */
    void setRestitution(float set_restitution);

    /**
     * Récupère la couche d'affichage de l'objet
     */
    unsigned int getLayer();

    /**
     * Modifie la couche d'affichage de l'objet
     */
    void setLayer(unsigned int set_layer);
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
