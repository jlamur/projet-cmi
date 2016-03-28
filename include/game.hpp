#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "object.hpp"
#include "engine_state.hpp"
#include "resource_manager.hpp"

/**
 * La classe Game gère l'affichage et les objets
 * d'une partie de jeu
 */
class Game {
private:
    sf::Clock clock;
    sf::RenderWindow window;
    sf::View view;
    sf::Music music;

    EngineState state;
    ResourceManager resources;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update();

    /**
     * Dessine la scène du jeu couche par couche
     */
    void draw();

public:
    Game();

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
