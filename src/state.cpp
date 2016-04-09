#include "state.hpp"
#include "manager.hpp"

State::State(Manager& manager) : manager(manager) {}
State::~State() {}

Manager& State::getManager() {
    return manager;
}

ResourceManager& State::getResourceManager() {
    return manager.getResourceManager();
}

sf::RenderWindow& State::getWindow() {
    return manager.getWindow();
}

bool State::isKeyPressed(sf::Keyboard::Key key) const {
    return manager.isKeyPressed(key);
}
