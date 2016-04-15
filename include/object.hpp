#ifndef __SKIZZLE_OBJECT_HPP__
#define __SKIZZLE_OBJECT_HPP__

#include <SFML/Graphics.hpp>
#include <fstream>
#include <memory>
#include "collision.hpp"

class Level;
class Game;

class Object : public std::enable_shared_from_this<Object> {
public:
    typedef std::shared_ptr<Object> Ptr;
    typedef std::weak_ptr<Object> WeakPtr;

private:
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;

    bool selected;
    Object::WeakPtr last_activator;

    // propriétés internes, autorisées à être modifiées
    // même par des méthodes constantes
    mutable float inv_mass;

    float mass;
    float charge;
    float restitution;
    float static_friction;
    float dynamic_friction;
    int layer;

protected:
    /**
     * Calcule les forces appliquées à l'objet
     */
    virtual sf::Vector2f getForces(const Game& game) const;

    /**
     * Appelé lorsque l'objet est activé par un autre
     */
    virtual void activate(Game& game, Object::Ptr object) = 0;

    /**
     * Initialisation des propriétés communes à tous les objets
     * depuis le fichier donné dans l'objet donné
     */
    static void init(std::ifstream& file, Object::Ptr object);

public:
    Object();
    virtual ~Object();

    /**
     * Clone cet objet en un objet avec les mêmes propriétés
     */
    virtual Object::Ptr clone() const = 0;

    /**
     * Récupère la boîte englobante de l'objet
     */
    virtual sf::FloatRect getAABB() const = 0;

    /**
     * Récupère le rayon de l'objet
     */
    virtual float getRadius() const = 0;

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(Level& level) = 0;

    /**
     * Récupère l'identifiant de type de cet objet
     */
    virtual unsigned int getTypeId() const = 0;

    /**
     * Récupère le type de collision de cet objet
     */
    virtual CollisionType getCollisionType() const = 0;

    /**
     * Sauvegarde cet objet dans le fichier donné
     */
    virtual void save(std::ofstream& file) const = 0;

    /**
     * Met à jour la vitesse de l'objet selon les
     * forces qui lui sont appliquées
     */
    virtual void updateVelocity(const Game& game);

    /**
     * Met à jour la position de l'objet selon sa
     * vitesse actuelle
     */
    virtual void updatePosition();

    /**
     * Résolution de la collision entre cet objet
     * et l'objet passé en paramètre selon la normale
     * donnée
     */
    virtual void solveCollision(Game& game, Object::Ptr obj, const sf::Vector2f& normal);

    /**
     * Application de la correction positionnelle sur
     * cet objet et l'objet passé en paramètre après
     * une résolution de collision de profondeur donnée.
     * En raison de l'imprécision des flottants sur la machine,
     * les objets peuvent accumuler une erreur de positionnement
     * qui les fait "plonger" les uns dans les autres
     */
    virtual void positionalCorrection(Object::Ptr obj, const sf::Vector2f& normal, float depth);

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
     * Récupère si l'objet est sélectionné
     */
    bool isSelected() const;

    /**
     * Modifie si l'objet est sélectionné
     */
    void setSelected(bool set_selected);

    /**
     * Récupère le dernier object ayant activé cet objet
     */
    Object::WeakPtr getLastActivator();

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
	bool operator()(Object::Ptr const &t1, Object::Ptr const &t2) const;
};

#endif
