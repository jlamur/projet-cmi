#ifndef __PTF_OBJECT_HPP__
#define __PTF_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <memory>
#include "collision.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"

typedef std::shared_ptr<Object> ObjectPtr;

class Object {
private:
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;

    sf::VertexArray acceleration_line;
    sf::VertexArray velocity_line;

    float mass;
    mutable float inv_mass;
    float charge;
    float restitution;
    float static_friction;
    float dynamic_friction;
    int layer;

protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(const Manager& manager, const std::vector<ObjectPtr>& objects) const;

public:
    Object(float x, float y);
    virtual ~Object();

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(Manager& manager);

    /**
     * Met à jour la vitesse de l'objet selon les
     * forces qui lui sont appliquées
     */
    void updateVelocity(const Manager& manager, const std::vector<ObjectPtr>& objects, float delta);

    /**
     * Met à jour la position de l'objet selon sa
     * vitesse actuelle
     */
    void updatePosition(float delta);

    /**
     * Détecte s'il y a collision entre cet objet
     * et l'objet passé en paramètre
     */
    bool detectCollision(const Object& obj, CollisionData& data) const;

    /**
     * Résolution de la collision entre cet objet
     * et l'objet passé en paramètre selon la normale
     * donnée
     */
    void solveCollision(Object& obj, const sf::Vector2f& normal);

    /**
     * Application de la correction positionnelle sur
     * cet objet et l'objet passé en paramètre après
     * une résolution de collision de profondeur donnée.
     * En raison de l'imprécision des flottants sur la machine,
     * les objets peuvent accumuler une erreur de positionnement
     * qui les fait "plonger" les uns dans les autres
     */
    void positionalCorrection(Object& obj, const sf::Vector2f& normal, float depth);

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual std::unique_ptr<sf::FloatRect> getAABB() const = 0;

    /**
     * Récupère l'identifiant de type de cet objet
     */
    virtual unsigned int getTypeId() const = 0;

    /**
     * Récupère l'accélération de l'objet
     */
    sf::Vector2f getAcceleration() const;

    /**
     * Récupère la vitesse de l'objet
     */
    sf::Vector2f getVelocity() const;

    /**
     * Modifie la vitesse de l'objet
     * (à utiliser avec précaution, préférer modifier les forces)
     */
    void setVelocity(sf::Vector2f set_velocity);

    /**
     * Récupère la position de l'objet
     */
    sf::Vector2f getPosition() const;

    /**
     * Modifie la position de l'objet
     * (à utiliser avec précaution, préférer modifier les forces)
     */
    void setPosition(sf::Vector2f set_position);

    /**
     * Récupère la masse de l'objet
     */
    float getMass() const;

    /**
     * Récupère l'inverse de la masse de l'objet (en cache)
     */
    float getMassInvert() const;

    /**
     * Modifie la masse de l'objet
     */
    void setMass(float set_mass);

    /**
     * Récupère la charge de l'objet
     */
    float getCharge() const;

    /**
     * Modifie la charge de l'objet
     */
    void setCharge(float set_charge);

    /**
     * Récupère le coefficient de restitution de l'objet
     */
    float getRestitution() const;

    /**
     * Modifie le coefficient de restitution de l'objet
     */
    void setRestitution(float set_restitution);

    /**
     * Récupère le coefficient de frottement dynamique de l'objet
     */
    float getStaticFriction() const;

    /**
     * Modifie le coefficient de frottement dynamique de l'objet
     */
    void setStaticFriction(float set_static_friction);

    /**
     * Récupère le coefficient de frottement dynamique de l'objet
     */
    float getDynamicFriction() const;

    /**
     * Modifie le coefficient de frottement dynamique de l'objet
     */
    void setDynamicFriction(float set_dynamic_friction);

    /**
     * Récupère la couche d'affichage de l'objet
     */
    int getLayer() const;

    /**
     * Modifie la couche d'affichage de l'objet
     */
    void setLayer(int set_layer);
};

/**
 * Classe de comparaison des couches deux objets
 * Renvoie "true" si le premier objet est sur une couche
 * qui doit être dessinée avant celle du second
 */
struct ObjectCompare {
	bool operator()(ObjectPtr const &t1, ObjectPtr const &t2) const;
};

#endif
