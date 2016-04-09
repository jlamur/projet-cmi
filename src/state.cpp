#include "state.hpp"
#include "manager.hpp"

State::State(Manager& manager) : manager(manager) {}
State::~State() {}

void State::frame(const std::vector<sf::Event>& events) {
    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        processEvent(events[i]);
    }
}

Manager& State::getManager() {
    return manager;
}

// raccourcis vers des méthodes du gestionnaire
ResourceManager& State::getResourceManager() {
    return manager.getResourceManager();
}

sf::RenderWindow& State::getWindow() {
    return manager.getWindow();
}

bool State::isKeyPressed(sf::Keyboard::Key key) const {
    return manager.isKeyPressed(key);
}
