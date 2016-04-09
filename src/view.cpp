#include "view.hpp"

View::View(Manager& manager) : manager(manager) {}
View::~View() {}

void View::frame(const std::vector<sf::Event>& events) {
    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        processEvent(events[i]);
    }
}
