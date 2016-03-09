#include "ball.hpp"
#include "engine.hpp"

int main() {
    Engine engine;
    Ball ball(0, 0);

    engine.addObject(&ball);
    engine.start();

    return 0;
}
