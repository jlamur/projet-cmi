#ifndef PTF_ENGINE_HPP
#define PTF_ENGINE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "ball.hpp"

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

public:
    static constexpr float GRAVITY = 10;
    static constexpr float ATTRACTION = 10;
    static constexpr float MOVE = 10;

    std::vector<Ball> balls;
    std::vector<std::vector<Block>> blocks;

    Engine();
    void draw();
    void update(float delta);
};

#endif
