#ifndef __PTF_ENGINE_HPP__
#define __PTF_ENGINE_HPP__

#include <SFML/Graphics.hpp>
#include "object.hpp"
#include "engine_state.hpp"

/**
 * La classe principale Engine coordonne les éléments
 * du jeu et organise le dessin des frames
 */
class Engine {
private:
    sf::Clock clock;
    sf::RenderWindow window;
    EngineState state;
    sf::View view; 

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état du moteur
     */
    void update();

    /**
     * Dessine la scène du jeu couche par couche
     */
    void draw();

public:
    Engine();

    /**
     * Démarre la boucle principale du jeu
     */
    void start();

    /**
     * Ajoute un nouvel objet à la liste des objets du jeu
     */
    void addObject(Object& object);
};

#endif
