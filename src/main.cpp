#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include "constants.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball1(3.5f * Constants::GRID, 10 * Constants::GRID);
    Ball ball2(4.5f * Constants::GRID, 10 * Constants::GRID);
    Block block01(2 * Constants::GRID, 10 * Constants::GRID);
    Block block02(2 * Constants::GRID, 11 * Constants::GRID);
    Block block03(3 * Constants::GRID, 11 * Constants::GRID);
    Block block04(4 * Constants::GRID, 11 * Constants::GRID);
    Block block05(5 * Constants::GRID, 11 * Constants::GRID);
    Block block06(6 * Constants::GRID, 11 * Constants::GRID);
    Block block07(7 * Constants::GRID, 11 * Constants::GRID);
    Block block08(8 * Constants::GRID, 11 * Constants::GRID);
    Block block09(9 * Constants::GRID, 11 * Constants::GRID);
    Block block10(10 * Constants::GRID, 11 * Constants::GRID);
    Block block11(11 * Constants::GRID, 11 * Constants::GRID);
    Block block12(12 * Constants::GRID, 11 * Constants::GRID);
    Block block13(13 * Constants::GRID, 11 * Constants::GRID);
    Block block14(14 * Constants::GRID, 11 * Constants::GRID);
    Block block15(15 * Constants::GRID, 11 * Constants::GRID);
    Block block16(16 * Constants::GRID, 11 * Constants::GRID);
    Block block17(17 * Constants::GRID, 11 * Constants::GRID);
    Block block18(18 * Constants::GRID, 11 * Constants::GRID);
    Block block19(19 * Constants::GRID, 11 * Constants::GRID);
    Block block20(20 * Constants::GRID, 10 * Constants::GRID);
    Block block21(20 * Constants::GRID, 11 * Constants::GRID);
    Block block22(19 * Constants::GRID, 10 * Constants::GRID);

    ball1.setCharge(-.01f);
    ball2.setCharge(-.01f);
    block22.setCharge(1.f);
    block22.setMass(2);

    engine.addObject(ball1);
    engine.addObject(ball2);
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

    engine.start();
    return 0;
}
