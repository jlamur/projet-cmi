#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), window_view(window.getView()), title(sf::String(L"")), view(NULL) {}

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

            if (event.type == sf::Event::Resized) {
                default_view = sf::View(sf::FloatRect(
                    0, 0, event.size.width, event.size.height
                ));
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

std::shared_ptr<View> Manager::getView() {
    return view;
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

void Manager::resetDefaultView() {
    window.setView(default_view);
}

sf::String Manager::getTitle() {
    return title;
}

void Manager::setTitle(sf::String set_title) {
    title = set_title;

    if (title.isEmpty()) {
        window.setTitle(sf::String(L"Skizzle"));
    } else {
        window.setTitle(sf::String(L"Skizzle ‒ ") + title);
    }
}

bool Manager::isKeyPressed(sf::Keyboard::Key key) const {
    return sf::Keyboard::isKeyPressed(key) && window.hasFocus();
}
