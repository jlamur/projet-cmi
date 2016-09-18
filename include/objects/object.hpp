#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "../level_data.hpp"
#include "geometry.hpp"

class Game;

/**
 * Représente de manière abstraite un des objets du jeu.
 * Les classes descendantes doivent notamment implémenter la
 * méthode d'activation, de dessin et de boîte englobante
 */
class Object {
public:
    /**
     * Initialise l'objet avec ses propriétés par défaut et
     * positionné sur le point donné
     */
    Object(sf::Vector2f position = sf::Vector2f());

    /**
     * Clone cet objet en un objet avec les mêmes propriétés
     */
    virtual LevelData::ObjectPtr clone() const = 0;

    /**
     * Sauvegarde cet objet dans le fichier donné
     */
    virtual void save(std::ofstream& file) const;

    /**
     * Dessine l'objet dans la fenêtre donnée
     */
    virtual void draw(sf::RenderWindow&) = 0;

    /**
     * Recalcule les forces appliquées à l'objet, en déduit son
     * accélération et l'intègre dans sa vitesse
     */
    virtual void updateVelocity(const Game& game);

    /**
     * Intègre la vitesse de l'objet dans sa position
     */
    virtual void updatePosition();

    /**
     * Résout la collision entre cet objet et l'objet donné selon la
     * normale donnée
     */
    virtual void solveCollision(Game& game, Object& obj, const sf::Vector2f& normal);

    /**
     * Applique la correction positionnelle sur cet objet et l'objet donné
     * après avoir résolu une collision de normale et profondeur données.
     * Ceci corrige l'effet de "plongement" d'un objet dans l'autre dû aux
     * erreurs d'arrondi
     */
    virtual void positionalCorrection(Object& obj, const sf::Vector2f& normal, float depth);

    /**
     * Récupère l'identifiant de type de cet objet. Chaque classe dérivée
     * définit son identifiant unique de type permettant son stockage et
     * sa lecture depuis un fichier
     *
     * (Lecture seule.)
     */
    virtual unsigned int getTypeId() const = 0;

    /**
     * Récupère la géométrie de l'objet, définie indépendamment
     * par chaque objet dérivé. Est utilisée pour calculer les
     * informations concernant les collisions entre deux objets
     *
     * (Lecture seule.)
     */
    virtual Geometry getGeometry() const = 0;

    /**
     * L'accélération de l'objet est calculée directement selon sa position dans
     * le plan, les objets qui l'entourent et ses propres propriétés
     *
     * (Lecture seule.)
     */
    sf::Vector2f getAcceleration() const;

    /**
     * La vitesse de l'objet est dérivée de son accélération à chaque
     * appel de Object#updateVelocity()
     *
     * (Lecture seule.)
     */
    sf::Vector2f getVelocity() const;

    /**
     * La position de l'objet est un vecteur qui pointe sur l'endroit
     * dans le plan où se situe l'objet. Une fois définie à l'initialisation,
     * la position de l'objet ne peut être modifiée qu'en modifiant les forces
     * qui s'y appliquent ou sa vitesse. La position est dérivée de la
     * vitesse à chaque appel de Object#updatePosition()
     *
     * (Lecture seule.)
     */
    sf::Vector2f getPosition() const;

    /**
     * L'objet peut être sélectionné, notamment dans l'éditeur de niveau.
     * Ce drapeau définit si l'objet apparaît sélectionné et est indépendant
     * de la sélection effective qui est gérée par la vue éditeur
     *
     * (Lecture et écriture.)
     */
    bool isSelected() const;
    void setSelected(bool);

    /**
     * La masse de l'objet est proportionnelle à la norme de la force
     * d'attraction qui lui est appliquée. La direction de la force de
     * gravité est variable selon le moment du jeu
     *
     * (Lecture, écriture et accès à l'inverse de la masse.)
     */
    float getMass() const;
    float getMassInvert() const;
    void setMass(float);

    /**
     * La charge de l'objet permet le calcul de la force d'attraction.
     * Le produit des charges de deux objets indique leur comportement.
     * Un produit positif fait se repousser les deux objets, un produit
     * négatif les fait s'attirer et un produit nul n'entraîne aucune
     * interaction
     *
     * (Lecture et écriture.)
     */
    float getCharge() const;
    void setCharge(float);

    /**
     * Le coefficient de restitution de l'objet indique sa capacité
     * à rebondir ou faire rebondir des objets qui entrent en collision
     * avec lui à une vitesse importante
     *
     * (Lecture et écriture.)
     */
    float getRestitution() const;
    void setRestitution(float);

    /**
     * Le coefficient de frottement statique de l'objet régit sa
     * capacité à empêcher la mise en mouvement des objets en contact
     * avec lui lorsqu'ils sont statiques
     *
     * (Lecture et écriture.)
     */
    float getStaticFriction() const;
    void setStaticFriction(float);

    /**
     * Le coefficient de frottement dynamique de l'objet régit sa
     * capacité à amoindrir la vitesse des objets qui génèrent
     * des frottements avec lui, dans le cas des objets qui sont
     * déjà en mouvement
     *
     * (Lecture et écriture.)
     */
    float getDynamicFriction() const;
    void setDynamicFriction(float);

    /**
     * La couche d'affiche d'un objet définit sa priorité d'affichage
     * parmi les autres objets. Les objets sur les couches supérieures
     * s'affichent au dessus des autres dans le cas où ils se chevauchent
     *
     * (Lecture et écriture.)
     */
    int getLayer() const;
    void setLayer(int);

protected:
    /**
     * Calcule les forces à appliquer à l'objet. La classe racine
     * définit un ensemble par défaut de forces. Les classes filles
     * peuvent ajouter leur propre force
     */
    virtual sf::Vector2f getForces(const Game& game) const;

    /**
     * Appelé lorsque l'objet est activé par un autre. Le comportement
     * par défaut est de ne rien faire, mais les classes filles peuvent
     * définir le leur
     */
    virtual void activate(Game& game, Object& object) = 0;

    /**
     * Lit les propriétés communes à tous les objets depuis le flux de
     * fichier donné et les applique à l'objet actuel
     */
    void read(std::ifstream& file);

    /**
     * Lit une propriété de l'objet depuis le fichier donné.
     * Les classes filles peuvent redéfinir cette méthode pour ajouter
     * la lecture de leurs propriétés
     */
    virtual bool readProperty(unsigned int, std::ifstream& file);

    /**
     * Écrit les propriétés de l'objet dans le fichier donné, si nécessaire.
     * Les classes filles peuvent redéfinir cette méthode pour ajouter
     * l'écriture de leurs propriétés
     */
    virtual void writeProperties(std::ofstream& file) const;

private:
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;

    bool selected;
    float mass;
    float inv_mass;
    float charge;
    float restitution;
    float static_friction;
    float dynamic_friction;
    int layer;
};

/**
 * Compare deux objets pour obtenir leur ordre d'affichage.
 * Renvoie vrai si l'objet gauche s'affiche sous l'objet droite
 * et faux s'il s'affiche au dessus
 */
struct ObjectCompare {
	bool operator()(const Object& t1, const Object& t2) const;
};
