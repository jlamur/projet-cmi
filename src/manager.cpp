#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Projet CMI", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), elapsed_time(0.f), view(NULL) {
    keys.fill(false);
}

void Manager::start() {
    while (window.isOpen()) {
        sf::Event event;

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // suivi de l'enfoncement et du relâchement des touches
            if (event.type == sf::Event::KeyPressed) {
                keys[event.key.code] = true;
            }

            if (event.type == sf::Event::KeyReleased) {
                keys[event.key.code] = false;
            }
        }

        // demande à la vue de se mettre à jour sur
        // la prochaine frame
        if (view == NULL) {
            throw std::runtime_error("Aucune vue à afficher pour le jeu");
        }

        elapsed_time = clock.restart().asSeconds();
        view->frame(*this);
    }
}

sf::RenderWindow& Manager::getWindow() {
    return window;
}

float Manager::getElapsedTime() const {
    return elapsed_time;
}

ResourceManager& Manager::getResourceManager() {
    return resource_manager;
}

bool Manager::isKeyPressed(sf::Keyboard::Key key) const {
    return keys[key];
}
