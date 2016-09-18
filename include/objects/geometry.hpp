#pragma once

#include <SFML/Graphics.hpp>

class Object;

class Geometry {
public:
    /**
     * Liste des géométries gérées par le moteur
     */
    enum class Type {CIRCLE, RECTANGLE};

    /**
     * Initialise la géométrie de l'objet comme étant
     * un cercle de rayon donné à la position donnée
     */
    Geometry(sf::Vector2f position, float radius);

    /**
     * Initialise la géométrie de l'objet comme étant
     * un rectangle de dimensions et position données
     */
    Geometry(sf::Vector2f position, sf::Vector2f size);

    /**
     * Vérifie si l'objet actuel s'intersecte avec l'objet
     * donné (signalant potentiellement une collision)
     */
    bool intersects(Geometry);

    /**
     * Vérifie s'il y a collision entre l'objet actuel et l'objet donné.
     * Si oui, renseigne la normale et la profondeur de la collision
     * et renvoie vrai ; sinon, renvoie faux
     *
     * (Statique.)
     */
    bool collides(Geometry, sf::Vector2f& normal, float& depth);

private:
    Type type;
    sf::Vector2f position;
    float radius;
    sf::FloatRect rect;

    bool circleToRectangle(Geometry, sf::Vector2f& normal, float& depth);
    bool rectangleToCircle(Geometry, sf::Vector2f& normal, float& depth);
    bool circleToCircle(Geometry, sf::Vector2f& normal, float& depth);
    bool rectangleToRectangle(Geometry, sf::Vector2f& normal, float& depth);
};
