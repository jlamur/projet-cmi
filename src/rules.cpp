#include "rules.hpp"
#include "manager.hpp"

Rules::Rules(Manager& manager) : State(manager) {
    background.setTexture(getResourceManager().getTexture("bg_rules.png"));
    getWindow().setFramerateLimit(Manager::FPS);
}

Rules::~Rules() {}

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
    sf::RenderWindow& window = getWindow();
    sf::Vector2f size = (sf::Vector2f) window.getSize();

    // titre de la fenêtre
    getManager().setTitle(L"Règles");
    getResourceManager().playMusic("menu.ogg");
    getWindow().setFramerateLimit(Manager::FPS);

    // on s'assure d'être dans la vue par défaut (pas de zoom, 0x0 en haut gauche)
    getManager().useGUIView();

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
