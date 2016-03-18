#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include "constants.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball(3.5f * Constants::GRID, 10 * Constants::GRID);
    Block block01(2 * Constants::GRID, 10 * Constants::GRID);
    Block block02(2 * Constants::GRID, 11 * Constants::GRID);
    Block block03(3 * Constants::GRID, 11 * Constants::GRID);
    Block block04(4 * Constants::GRID, 11 * Constants::GRID);
    Block block05(5 * Constants::GRID, 11 * Constants::GRID);
    Block block06(6 * Constants::GRID, 11 * Constants::GRID);
    Block block07(7 * Constants::GRID, 11 * Constants::GRID);
    Block block08(8 * Constants::GRID, 11 * Constants::GRID);
    Block block09(9 * Constants::GRID, 11 * Constants::GRID);
    Block block10(13 * Constants::GRID, 11 * Constants::GRID);
    Block block11(14 * Constants::GRID, 11 * Constants::GRID);
    Block block12(15 * Constants::GRID, 11 * Constants::GRID);
    Block block13(16 * Constants::GRID, 11 * Constants::GRID);
    Block block14(17 * Constants::GRID, 11 * Constants::GRID);
    Block block15(18 * Constants::GRID, 11 * Constants::GRID);
    Block block16(19 * Constants::GRID, 11 * Constants::GRID);
    Block block17(20 * Constants::GRID, 10 * Constants::GRID);
    Block block18(20 * Constants::GRID, 11 * Constants::GRID);
    Block block19(14 * Constants::GRID, 6 * Constants::GRID);
    Block block20(13 * Constants::GRID, 6 * Constants::GRID);
    Block block21(12 * Constants::GRID, 6 * Constants::GRID);
    Block block22(11 * Constants::GRID, 6 * Constants::GRID);
    Block block23(10 * Constants::GRID, 6 * Constants::GRID);
    Block block24(9 * Constants::GRID, 6 * Constants::GRID);
    Block block25(8 * Constants::GRID, 6 * Constants::GRID);

    ball.setCharge(-1.f);
    block23.setCharge(1.f);
    block21.setCharge(1.f);

    engine.addObject(ball);
    engine.addObject(block01);
    engine.addObject(block02);
    engine.addObject(block03);
    engine.addObject(block04);
    engine.addObject(block05);
    engine.addObject(block06);
    engine.addObject(block07);
    engine.addObject(block08);
    engine.addObject(block09);
    engine.addObject(block10);
    engine.addObject(block11);
    engine.addObject(block12);
    engine.addObject(block13);
    engine.addObject(block14);
    engine.addObject(block15);
    engine.addObject(block16);
    engine.addObject(block17);
    engine.addObject(block18);
    engine.addObject(block19);
    engine.addObject(block20);
    engine.addObject(block21);
    engine.addObject(block22);
    engine.addObject(block23);
    engine.addObject(block24);
    engine.addObject(block25);

    engine.start();
    return 0;
}
