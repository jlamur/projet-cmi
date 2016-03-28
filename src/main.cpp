#include "player.hpp"
#include "block.hpp"
#include "game.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <iostream>

int main() {
    Game game;

    Player player1(3.5f * Constants::GRID, 10 * Constants::GRID);
    player1.setPlayerNumber(1);
    Player player2(18.5f * Constants::GRID, 10 * Constants::GRID);
    player2.setPlayerNumber(2);
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
    Block block22(11 * Constants::GRID, 10 * Constants::GRID);

    player1.setCharge(-.01f);
    player2.setCharge(-.01f);
    block22.setCharge(1.f);
    block22.setMass(2);

    game.addObject(player1);
    game.addObject(player2);
    game.addObject(block01);
    game.addObject(block02);
    game.addObject(block03);
    game.addObject(block04);
    game.addObject(block05);
    game.addObject(block06);
    game.addObject(block07);
    game.addObject(block08);
    game.addObject(block09);
    game.addObject(block10);
    game.addObject(block11);
    game.addObject(block12);
    game.addObject(block13);
    game.addObject(block14);
    game.addObject(block15);
    game.addObject(block16);
    game.addObject(block17);
    game.addObject(block18);
    game.addObject(block19);
    game.addObject(block20);
    game.addObject(block21);
    game.addObject(block22);

    try {
        game.start();
    } catch (const std::exception& exception) {
        std::cerr << std::endl;
        std::cerr << "Le programme a quitté après une erreur d'exécution." << std::endl;
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
