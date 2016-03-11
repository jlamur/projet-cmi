#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball(0, 0);
    Block block1(0 * Block::GRID, 3 * Block::GRID);
    Block block2(1 * Block::GRID, 3 * Block::GRID);
    Block block3(2 * Block::GRID, 3 * Block::GRID);
    Block block4(3 * Block::GRID, 3 * Block::GRID);

    ball.setCharge(100);
    block4.setCharge(-100);

    engine.addObject(&ball);
    engine.addObject(&block1);
    engine.addObject(&block2);
    engine.addObject(&block3);
    engine.addObject(&block4);

    engine.start();
    return 0;
}
