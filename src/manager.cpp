#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), window_view(window.getView()), title(""), view(NULL) {}

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
                setWindowView(sf::View(visibleArea));
            }

            events.push_back(event);
        }

        // demande à la vue de se mettre à jour sur
        // la prochaine frame
        if (view == NULL) {
            throw std::runtime_error("Aucune vue à afficher pour le jeu");
        }

        view->frame();
        window.display();
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

sf::View Manager::getWindowView() {
    return window_view;
}

void Manager::setWindowView(sf::View set_window_view) {
    window.setView(set_window_view);
    window_view = set_window_view;
}

std::string Manager::getTitle() {
    return title;
}

void Manager::setTitle(std::string set_title) {
    title = set_title;

    if (title.empty()) {
        window.setTitle("Skizzle");
    } else {
        window.setTitle("Skizzle - " + title);
    }
}

bool Manager::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}
