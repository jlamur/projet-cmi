#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include "constants.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball1(5 * Constants::GRID, 1 * Constants::GRID);
    Ball ball2(7 * Constants::GRID, 1 * Constants::GRID);

    Block block1(2 * Constants::GRID, 7 * Constants::GRID);
    Block block2(3 * Constants::GRID, 7 * Constants::GRID);
    Block block3(4 * Constants::GRID, 7 * Constants::GRID);
    Block block4(5 * Constants::GRID, 7 * Constants::GRID);
    Block block5(6 * Constants::GRID, 7 * Constants::GRID);
    Block block6(7 * Constants::GRID, 7 * Constants::GRID);
    Block block7(8 * Constants::GRID, 7 * Constants::GRID);
    Block block8(9 * Constants::GRID, 7 * Constants::GRID);
    Block block9(10 * Constants::GRID, 7 * Constants::GRID);

    ball1.setCharge(-2.f);
    ball2.setCharge(-2.f);
    block5.setCharge(16.f);

    engine.addObject(ball1);
    engine.addObject(ball2);
    engine.addObject(block1);
    engine.addObject(block2);
    engine.addObject(block3);
    engine.addObject(block4);
    engine.addObject(block5);
    engine.addObject(block6);
    engine.addObject(block7);
    engine.addObject(block8);
    engine.addObject(block9);

    engine.start();
    return 0;
}
