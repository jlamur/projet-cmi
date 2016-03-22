#ifndef __PTF_OBJECT_HPP__
#define __PTF_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "engine_state.hpp"
#include "resource_manager.hpp"

class Block;
class Ball;

class Object {
private:
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;

    sf::VertexArray acceleration_line;
    sf::VertexArray velocity_line;

    float mass;
    float inv_mass;
    float charge;
    float restitution;
    float static_friction;
    float dynamic_friction;
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
    virtual void draw(sf::RenderWindow& window, ResourceManager& resources);

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
     * Calcule les informations sur une éventuelle collision de
     * cet objet avec un autre : la normale et la profondeur
     */
    virtual bool getCollisionInfo(Object& obj, sf::Vector2f& normal, float& depth);
    virtual bool getCollisionInfo(Ball& obj, sf::Vector2f& normal, float& depth) = 0;
    virtual bool getCollisionInfo(Block& obj, sf::Vector2f& normal, float& depth) = 0;

    /**
     * Détecte s'il y a collision entre cet objet
     * et l'objet passé en paramètre et résoud la collision
     * si elle a lieu
     */
    void collide(Object& obj);

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
     * Modifie la position de l'objet
     * (à utiliser avec précaution, préférer modifier les forces)
     */
    void setPosition(sf::Vector2f set_position);

    /**
     * Récupère la masse de l'objet
     */
    float getMass();

    /**
     * Récupère l'inverse de la masse de l'objet (en cache)
     */
    float getMassInvert();

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
     * Récupère le coefficient de frottement dynamique de l'objet
     */
    float getStaticFriction();

    /**
     * Modifie le coefficient de frottement dynamique de l'objet
     */
    void setStaticFriction(float set_static_friction);

    /**
     * Récupère le coefficient de frottement dynamique de l'objet
     */
    float getDynamicFriction();

    /**
     * Modifie le coefficient de frottement dynamique de l'objet
     */
    void setDynamicFriction(float set_dynamic_friction);

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
