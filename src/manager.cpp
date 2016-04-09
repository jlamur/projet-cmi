#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), default_view(window.getDefaultView()), title(sf::String(L"")),
view(NULL), next_view(NULL), running(false) {}

void Manager::start() {
    running = true;

    while (running) {
        sf::Event event;
        std::vector<sf::Event> events;

        // si un changement de vue a été demandé, on l'effectue maintenant
        if (next_view != nullptr) {
            view = next_view;
            next_view->begin();
            next_view = nullptr;
        }

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre
            if (event.type == sf::Event::Closed) {
                quit();
            }

            // redimensionnement de la vue par défaut
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

        view->frame(events);
        window.display();
    }
}

void Manager::quit() {
    running = false;
}

std::shared_ptr<View> Manager::getView() {
    return view;
}

void Manager::setView(std::shared_ptr<View> set_view) {
    // on ne change pas immédiatement la vue, on attend
    // la prochaine frame pour éviter toute erreur de segmentation
    next_view = set_view;
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

void Manager::resetDefaultView() {
    window.setView(default_view);
}

sf::String Manager::getTitle() {
    return title;
}

void Manager::setTitle(sf::String set_title) {
    if (title == set_title) {
        return;
    }

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
