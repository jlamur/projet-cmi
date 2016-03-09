#include "ball.hpp"
#include "engine.hpp"

int main() {
    Engine engine;
    Ball ball;

    engine.addObject(&ball);
    engine.start();

    return 0;
}
