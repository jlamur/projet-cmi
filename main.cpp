#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball(0, 0);
    Block block1(0, 3);
    Block block2(1, 3);
    Block block3(2, 3);
    Block block4(3, 3);

    engine.addObject(&ball);
    engine.addObject(&block1);
    engine.addObject(&block2);
    engine.addObject(&block3);
    engine.addObject(&block4);

    engine.start();
    return 0;
}
