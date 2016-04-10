#include "manager.hpp"

const unsigned int Manager::FPS = 60;
const sf::Time Manager::FRAME_TIME = sf::seconds(1.f / Manager::FPS);

Manager::Manager() : title(sf::String(L"")), state(NULL), next_state(NULL), running(false) {
    // préchargement des textures
    resource_manager.preload();

    // création de la fenêtre (après avoir préchargé les ressources,
    // on évite ainsi tout lag pendant le traitement des événements)
    window.create(
        sf::VideoMode(704, 480), "Skizzle", sf::Style::Default,
        sf::ContextSettings(0, 0, 2)
    );

    // récupération de la vue par défaut comme vue du gui
    gui_view = window.getDefaultView();
}

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
                gui_view = sf::View(sf::FloatRect(
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

void Manager::useGUIView() {
    window.setView(gui_view);
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

bool Manager::isKeyPressed(Manager::Modifier modifier) const {
    switch (modifier) {
    case Manager::Modifier::CONTROL:
        return isKeyPressed(sf::Keyboard::LControl) || isKeyPressed(sf::Keyboard::RControl);

    case Manager::Modifier::ALT:
        return isKeyPressed(sf::Keyboard::LAlt) || isKeyPressed(sf::Keyboard::RAlt);

    case Manager::Modifier::SYSTEM:
        return isKeyPressed(sf::Keyboard::LSystem) || isKeyPressed(sf::Keyboard::RSystem);

    case Manager::Modifier::SHIFT:
        return isKeyPressed(sf::Keyboard::LShift) || isKeyPressed(sf::Keyboard::RShift);
    }

    return false;
}
