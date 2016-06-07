#include "manager.hpp"
#include "resource_manager.hpp"
#include "states/rules.hpp"

Rules::Rules(Manager& manager) : State(manager) {
    background.setTexture(*ResourceManager::get().getTexture("rules.tga"));
}

Rules::~Rules() {}

void Rules::enable() {
    // attributs de la fenêtre
    getManager().setTitle(L"Règles");
    getManager().getWindow().setFramerateLimit(Manager::FPS);

    // joue la musique du menu
    ResourceManager::get().playMusic("menu.ogg");
}

void Rules::processEvent(const sf::Event& event) {
    // appui sur échap, retour, entrée, espace : on dépile l'état
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::BackSpace ||
            event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
            getManager().popState();
        }
    }
}

void Rules::frame() {
    sf::RenderWindow& window = getManager().getWindow();
    sf::Vector2f size = (sf::Vector2f) window.getSize();

    // on s'assure d'être dans la vue canonique
    window.setView(getManager().getCanonicalView());

    // affichage du fond
    window.clear(sf::Color(22, 22, 22));

    // on met à l'échelle le fond de telle sorte qu'il soit toujours
    // entièrement visible en hauteur
    float bg_scale = size.y / 1100;
    background.setScale(bg_scale, bg_scale);

    background.setPosition(sf::Vector2f(
        size.x / 2 - background.getGlobalBounds().width / 2,
        size.y / 2 - 720 * bg_scale
    ));

    window.draw(background);
    window.display();
}
