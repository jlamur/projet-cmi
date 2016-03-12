#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball1(3 * Block::GRID, 1 * Block::GRID);
    Ball ball2(5 * Block::GRID, 1 * Block::GRID);

    Block block1(2 * Block::GRID, 7 * Block::GRID);
    Block block2(3 * Block::GRID, 7 * Block::GRID);
    Block block3(4 * Block::GRID, 7 * Block::GRID);
    Block block4(5 * Block::GRID, 7 * Block::GRID);
    Block block5(6 * Block::GRID, 7 * Block::GRID);

    ball1.setCharge(2);
    ball2.setCharge(-2);
    block3.setCharge(10);

    engine.addObject(ball1);
    engine.addObject(ball2);
    engine.addObject(block1);
    engine.addObject(block2);
    engine.addObject(block3);
    engine.addObject(block4);
    engine.addObject(block5);

    engine.start();
    return 0;
}