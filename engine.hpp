#ifndef PTF_ENGINE_HPP
#define PTF_ENGINE_HPP

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "state.hpp"
#include "object.hpp"

/**
 * La classe principale Engine coordonne les éléments
 * du jeu et organise le dessin des frames
 */
class Engine {
private:
    sf::Clock clock;
    sf::RenderWindow window;

    bool goLeftKey;
    bool goRightKey;

    std::vector<std::unique_ptr<Object>> objects;

public:
    static constexpr float GRAVITY = 10;
    static constexpr float ATTRACTION = 10;
    static constexpr float MOVE = 10;

    Engine();

    /**
     * Dessine la scène du jeu couche par couche
     */
    void draw();

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état du moteur
     */
    void update(State state);
};

#endif
