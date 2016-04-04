#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Projet CMI", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), view(NULL) {}

void Manager::start() {
    while (window.isOpen()) {
        sf::Event event;
        events.clear();

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // lorsque la fenêtre est redimensionnée par l'utilisateur
            if (event.type == sf::Event::Resized) {
                // mise à jour de la caméra en fonction de la taille de la fenêtre
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            events.push_back(event);
        }

        // demande à la vue de se mettre à jour sur
        // la prochaine frame
        if (view == NULL) {
            throw std::runtime_error("Aucune vue à afficher pour le jeu");
        }

        view->frame();
    }
}

void Manager::setView(std::shared_ptr<View> set_view) {
    view = set_view;
}

sf::RenderWindow& Manager::getWindow() {
    return window;
}

sf::Time Manager::getCurrentTime() const {
    return clock.getElapsedTime();
}

ResourceManager& Manager::getResourceManager() {
    return resource_manager;
}

const std::vector<sf::Event>& Manager::getEvents() {
    return events;
}

bool Manager::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}
