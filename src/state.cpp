#include "state.hpp"
#include "manager.hpp"

State::State(Manager& manager) : manager(manager) {}
State::~State() {}

Manager& State::getManager() {
    return manager;
}

const Manager& State::getManager() const {
    return manager;
}

ResourceManager& State::getResourceManager() {
    return manager.getResourceManager();
}

const ResourceManager& State::getResourceManager() const {
    return manager.getResourceManager();
}

sf::RenderWindow& State::getWindow() {
    return manager.getWindow();
}

const sf::RenderWindow& State::getWindow() const {
    return manager.getWindow();
}
