#include "object.hpp"

Object::Object(float x, float y) : position(x, y), charge(0.f), layer(10) {}

sf::Vector2f Object::getPosition() {
    return position;
}

unsigned int Object::getLayer() {
    return layer;
}

void Object::setLayer(unsigned int set_layer) {
    layer = set_layer;
}

float Object::getCharge() {
    return charge;
}

void Object::setCharge(float set_charge) {
    charge = set_charge;
}

bool ObjectCompare::operator()(Object* const &t1, Object* const &t2) {
    return t1->getLayer() > t2->getLayer();
}
