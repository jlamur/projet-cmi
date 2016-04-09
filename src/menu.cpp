#include "menu.hpp"
#include "editor.hpp"
#include "game.hpp"
#include <cmath>

const float MAX_WIDTH_PROPORTION = 1.f / 3.f;
const float PADDING = 24.f;

Menu::Menu(Manager& manager) : View(manager) {}
Menu::~Menu() {}

void Menu::begin() {
    ResourceManager& resources = manager.getResourceManager();

    loadMainMenu();
    resources.setMusic("menu.wav");
    resources.playMusic();

    manager.getWindow().setFramerateLimit(60);
}

void Menu::frame(const std::vector<sf::Event>& events) {
    // traitement des événements
    View::frame(events);

    // titre de la fenêtitre
    manager.setTitle("");

    // affichage du menu
    sf::RenderWindow& window = manager.getWindow();
    sf::Vector2f size = (sf::Vector2f) window.getSize();
    sf::Font font = manager.getResourceManager().getFont("raleway.ttf");

    window.clear(sf::Color(66, 40, 245));
    // TODO: dessiner l'image du fond

    // on crée les textes pour chaque choix et on les dessine
    float step = size.y / (choices.size() + 1);
    int font_size = std::max((int) std::floor(step - PADDING), 12);

    for (unsigned int i = 0; i < choices.size(); i++) {
        sf::Text label(choices[i], font, 32);
        sf::Vector2f position(
            (1 - MAX_WIDTH_PROPORTION) * size.x,
            step * (i + 1) - font_size / 2
        );

        float width = label.getGlobalBounds().width;
        label.setPosition(position);
        label.setColor(sf::Color::White);

        // si c'est le choix sélecitonné, on le souligne
        if (selection == i) {
            sf::RectangleShape underline(sf::Vector2f(width, 2.f));
            underline.setPosition(position.x, position.y + font_size / 2 + 6);
            underline.setFillColor(sf::Color::White);
            window.draw(underline);
        }

        window.draw(label);
    }

    window.display();
}

void Menu::processEvent(const sf::Event& event) {
    // gestion des touches
    if (event.type == sf::Event::KeyPressed) {
        // touche flèche haut : on passe au choix précédent
        if (event.key.code == sf::Keyboard::Up) {
            if (selection == 0) {
                selection = choices.size() - 1;
            } else {
                selection--;
            }
        }

        // touche flèche bas : on passe au choix suivant
        if (event.key.code == sf::Keyboard::Down) {
            if (selection == choices.size()) {
                selection = 0;
            } else {
                selection++;
            }
        }

        // touche entrée : on exécute le choix sélectionné
        if (event.key.code == sf::Keyboard::Return) {
            actions[selection]();
        }
    }
}

void Menu::loadMainMenu() {
    choices.clear();
    actions.clear();
    selection = 0;

    choices.push_back(sf::String(L"Jouer"));
    actions.push_back(std::bind(&Menu::loadLevelMenu, this));

    choices.push_back(sf::String(L"Règles du jeu"));
    actions.push_back(std::bind(&Menu::loadRules, this));

    choices.push_back(sf::String(L"Éditeur"));
    actions.push_back(std::bind(&Menu::launchEditor, this));

    choices.push_back(sf::String(L"Quitter"));
    actions.push_back(std::bind(&Menu::quit, this));
}

void Menu::loadLevelMenu() {
    choices.clear();
    actions.clear();
    selection = 0;

    choices.push_back(sf::String(L"Tutoriel"));
    actions.push_back(std::bind(&Menu::launchGame, this, "level1.dat"));

    choices.push_back(sf::String(L"Niveau 1"));
    actions.push_back(std::bind(&Menu::launchGame, this, "level2.dat"));

    choices.push_back(sf::String(L"Niveau 2"));
    actions.push_back(std::bind(&Menu::launchGame, this, "level3.dat"));

    choices.push_back(sf::String(L"Retour"));
    actions.push_back(std::bind(&Menu::loadMainMenu, this));
}

void Menu::loadRules() {
    // TODO: coder l'affichage des règles
}

void Menu::launchEditor() {
    std::shared_ptr<View> editor = std::shared_ptr<View>(new Editor(manager));
    manager.setView(editor);
}

void Menu::launchGame(std::string name) {
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(manager));
    std::string path = "./levels/" + name;

    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);
    game->load(file);
    file.close();

    manager.setView(game);
}

void Menu::quit() {
    manager.quit();
}
