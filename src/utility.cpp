#include "utility.hpp"
#include "manager.hpp"
#include <cmath>

float Utility::animateValue(float current, float speed, float goal) {
    float gap = goal - current;
    float diff = gap * Manager::FRAME_TIME.asSeconds() * speed;

    // si on est très proches de la fin, on termine
    if (std::abs(gap) < .05f) {
        return goal;
    }

    return current + diff;
}
