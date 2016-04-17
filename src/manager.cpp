#include "state.hpp"
#include "manager.hpp"

const unsigned int Manager::FPS = 60;
const sf::Time Manager::FRAME_TIME = sf::seconds(1.f / Manager::FPS);
const float Manager::GRID = 32;

Manager::Manager() : title(sf::String(L"")), previous_state(nullptr) {
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
    while (!states.empty()) {
        sf::Event event;

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre : on dépile tous les états
            if (event.type == sf::Event::Closed) {
                while (!states.empty()) {
                    states.pop();
                }
            }

            // redimensionnement de la vue par défaut
            if (event.type == sf::Event::Resized) {
                gui_view = sf::View(sf::FloatRect(
                    0, 0, event.size.width, event.size.height
                ));
            }

            // s'il n'y a plus d'état, on quitte
            if (states.empty()) {
                return;
            }

            states.top()->processEvent(event);
        }

        // s'il n'y a plus d'état, on quitte
        if (states.empty()) {
            return;
        }

        // si l'état que l'on va utiliser n'est pas le
        // même que précédemment, on l'active
        // Ceci permet un partage plus facile des
        // ressources globales (vue, musique)
        if (previous_state != states.top().get()) {
            previous_state = states.top().get();
            previous_state->enable();
        }

        // demande à l'état actuel d'afficher une frame
        states.top()->frame();
        window.display();
    }
}

void Manager::pushState(std::unique_ptr<State> set_state) {
    states.push(std::move(set_state));
}

void Manager::popState() {
    states.pop();
}

sf::RenderWindow& Manager::getWindow() {
    return window;
}

unsigned int Manager::getFramerate() {
    return framerate;
}

void Manager::setFramerate(unsigned int set_framerate) {
    // on ne modifie le framerate maximal que s'il a changé
    if (set_framerate != framerate) {
        window.setFramerateLimit(set_framerate);
        framerate = set_framerate;
    }
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
    if (title != set_title) {
        title = set_title;

        // màj du titre selon le titre donné par l'état
        if (title.isEmpty()) {
            window.setTitle(sf::String(L"Skizzle"));
        } else {
            window.setTitle(sf::String(L"Skizzle ‒ ") + title);
        }
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
