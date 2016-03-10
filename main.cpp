#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball(0, 0);
    Block block1(0, 5);
    Block block2(1, 5);
    Block block3(2, 5);
    Block block4(3, 5);

    engine.addObject(&ball);
    engine.addObject(&block1);
    engine.addObject(&block2);
    engine.addObject(&block3);
    engine.addObject(&block4);

    engine.start();
    return 0;
}
