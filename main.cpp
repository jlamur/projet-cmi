#include "ball.hpp"
#include "block.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    Ball ball(0, 0);
    std::vector<Block> blocks;

    engine.addObject(&ball);

    for (int i = 0; i < 10; i++) {
        Block block(i, 5);
        std::cout << "Pos: " << block.getPosition().x << " x " << block.getPosition().y << std::endl;

        blocks.push_back(block);
        engine.addObject(&block);
    }

    engine.start();
    return 0;
}
