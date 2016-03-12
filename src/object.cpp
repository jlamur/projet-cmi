#include "object.hpp"

sf::Vector2f Object::getPosition() {
    return position;
}

int Object::getCharge() {
    return charge;
}

void Object::setCharge(int set_charge) {
    charge = set_charge;
}
