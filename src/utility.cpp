#include <cmath>
#include "manager.hpp"
#include "utility.hpp"

float Utility::animateValue(float current, float speed, float goal) {
    float gap = goal - current;
    float diff = gap * Manager::FRAME_TIME.asSeconds() * speed;

    // si on est très proche de la fin, on termine
    if (std::abs(gap) < .1f) {
        return goal;
    }

    return current + diff;
}

bool Utility::isKeyPressed(sf::Keyboard::Key key) {
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}

bool Utility::isKeyPressed(Manager::Modifier modifier) {
    switch (modifier) {
    case Manager::Modifier::CONTROL:
        return isKeyPressed(sf::Keyboard::LControl) || isKeyPressed(sf::Keyboard::RControl);

    case Manager::Modifier::ALT:
        return isKeyPressed(sf::Keyboard::LAlt) || isKeyPressed(sf::Keyboard::RAlt);

    case Manager::Modifier::SYSTEM:
        return isKeyPressed(sf::Keyboard::LSystem) || isKeyPressed(sf::Keyboard::RSystem);

    case Manager::Modifier::SHIFT:
        return isKeyPressed(sf::Keyboard::LShift) || isKeyPressed(sf::Keyboard::RShift);
    }

    return false;
}
