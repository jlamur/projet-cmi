#include "ball.hpp"
#include "engine.hpp"

int main() {
    Engine engine;
    Ball ball;

    engine.balls.push_back(ball);
    engine.start();

    return 0;
}
