#include "object.hpp"

Object::Object(float x, float y) : position(x, y), charge(0) {}

sf::Vector2f Object::getPosition() {
    return position;
}

int Object::getCharge() {
    return charge;
}

void Object::setCharge(int set_charge) {
    charge = set_charge;
}

bool ObjectCompare::operator()(Object* const &t1, Object* const &t2) {
    return t1->getLayer() > t2->getLayer();
}
