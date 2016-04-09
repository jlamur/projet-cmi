#include "manager.hpp"

Manager::Manager() : window(
    sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), default_view(window.getDefaultView()), title(sf::String(L"")),
state(NULL), next_state(NULL), running(false) {}

void Manager::start() {
    running = true;

    while (running) {
        sf::Event event;

        // si un changement d'état a été demandé, on l'effectue maintenant
        if (next_state != nullptr) {
            state = next_state;
            next_state->begin();
            next_state = nullptr;
        }

        // vérification de sécurité
        if (state == NULL) {
            throw std::runtime_error("Aucun état à afficher pour le jeu");
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

            state->processEvent(event);
        }

        // affichage de la prochaine frame
        state->frame();
        window.display();
    }
}

void Manager::quit() {
    running = false;
}

std::shared_ptr<State> Manager::getState() {
    return state;
}

void Manager::setState(std::shared_ptr<State> set_state) {
    // on ne change pas immédiatement l'état, on attend
    // la prochaine frame pour éviter toute erreur de segmentation
    next_state = set_state;
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
