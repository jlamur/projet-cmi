#include "view.hpp"
#include "manager.hpp"

View::View(Manager& manager) : manager(manager) {}
View::~View() {}

void View::frame(const std::vector<sf::Event>& events) {
    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        processEvent(events[i]);
    }
}

Manager& View::getManager() {
    return manager;
}

// raccourcis vers des méthodes du gestionnaire
ResourceManager& View::getResourceManager() {
    return manager.getResourceManager();
}

sf::RenderWindow& View::getWindow() {
    return manager.getWindow();
}

bool View::isKeyPressed(sf::Keyboard::Key key) const {
    return manager.isKeyPressed(key);
}
