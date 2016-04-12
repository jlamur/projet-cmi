#include "manager.hpp"
#include "menu.hpp"
#include "rules.hpp"
#include "editor.hpp"
#include "game.hpp"
#include <cmath>

Menu::Menu(Manager& manager) : State(manager) {
    background.setTexture(getResourceManager().getTexture("bg_menu.tga"));
    loadMainMenu();
}

Menu::~Menu() {}

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
            if (selection == choices.size() - 1) {
                selection = 0;
            } else {
                selection++;
            }
        }

        // touche entrée ou espace : on exécute le choix sélectionné
        if (event.key.code == sf::Keyboard::Return ||
            event.key.code == sf::Keyboard::Space) {
            actions[selection]();
        }

        // touche retour ou échap : on revient au menu principal
        if (event.key.code == sf::Keyboard::BackSpace ||
            event.key.code == sf::Keyboard::Escape) {
            loadMainMenu();
        }
    }

    // au clic, on exécute le choix pointé s'il y a lieu
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

        for (unsigned int i = 0; i < labels.size(); i++) {
            if (labels[i].getGlobalBounds().contains(position)) {
                actions[i]();
                return;
            }
        }
    }

    // au déplacement de souris, on sélectionne le choix pointé s'il y a lieu
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f position(event.mouseMove.x, event.mouseMove.y);

        for (unsigned int i = 0; i < labels.size(); i++) {
            if (labels[i].getGlobalBounds().contains(position)) {
                selection = i;
            }
        }
    }
}

void Menu::frame() {
    // attributsde la fenêtre
    getManager().setTitle("");
    getManager().setFramerate(Manager::FPS);

    // joue la musique du menu
    getResourceManager().playMusic("menu.ogg");

    // affichage du menu
    sf::RenderWindow& window = getWindow();
    sf::Vector2f size = (sf::Vector2f) window.getSize();
    sf::Font font = getResourceManager().getFont("raleway.ttf");

    // on s'assure d'être dans la vue par défaut (pas de zoom, 0x0 en haut gauche)
    getManager().useGUIView();

    // dessin du fond
    window.clear(sf::Color::White);

    // on met à l'échelle le fond de telle sorte qu'il soit toujours
    // entièrement visible en largeur
    float bg_scale = size.x / background.getLocalBounds().width;
    background.setScale(bg_scale, bg_scale);

    background.setPosition(sf::Vector2f(
        0, size.y - background.getGlobalBounds().height
    ));

    window.draw(background);

    // on crée les textes pour chaque choix et on les dessine
    float step = size.y / (choices.size() + 1);
    int font_size = std::max((int) step / 3, 12);

    // d'abord un fond pour éviter de mélanger texte et image
    sf::Vector2f mask_position(size.x - (font_size * 10), 0);
    sf::RectangleShape mask(sf::Vector2f(font_size * 10, size.y));
    mask.setPosition(mask_position);
    mask.setFillColor(sf::Color(0, 0, 0, 127));

    window.draw(mask);
    labels.clear();

    for (unsigned int i = 0; i < choices.size(); i++) {
        sf::Text label(choices[i], font, font_size);
        sf::FloatRect text_size = label.getLocalBounds();

        sf::Vector2f base_position(
            size.x - (font_size * 8),
            step * (i + 1)
        );

        label.setColor(sf::Color::White);
        label.setPosition(base_position - sf::Vector2f(
            text_size.left, text_size.top + text_size.height / 2
        ));

        // si c'est le choix sélectionné, on le souligne
        if (selection == i) {
            sf::RectangleShape underline(sf::Vector2f(text_size.width, 2.f));
            underline.setFillColor(sf::Color::White);
            underline.setPosition(base_position + sf::Vector2f(
                0, text_size.height / 2 + 8
            ));

            window.draw(underline);
        }

        labels.push_back(label);
        window.draw(label);
    }

    window.display();
}

void Menu::loadMainMenu() {
    choices.clear();
    actions.clear();
    selection = 0;

    sf::Texture& texture = getResourceManager().getTexture("bg_menu.tga");
    texture.setSmooth(true);
    background.setTexture(texture);

    choices.push_back(sf::String(L"Jouer"));
    actions.push_back(std::bind(&Menu::loadLevelMenu, this));

    choices.push_back(sf::String(L"Règles du jeu"));
    actions.push_back(std::bind(&Menu::launchRules, this));

    choices.push_back(sf::String(L"Éditeur"));
    actions.push_back(std::bind(&Menu::loadEditorMenu, this));

    choices.push_back(sf::String(L"Quitter"));
    actions.push_back(std::bind(&Menu::quit, this));
}

void Menu::loadLevelMenu() {
    choices.clear();
    actions.clear();
    selection = 0;

    std::vector<std::string> path_list = getResourceManager().getLevelList();
    std::vector<std::string> name_list;

    for (auto it = path_list.begin(); it != path_list.end(); it++) {
        choices.push_back(Level::getLevelName(*it));
        actions.push_back(std::bind(&Menu::launchGame, this, *it));
    }

    choices.push_back(sf::String(L"Retour"));
    actions.push_back(std::bind(&Menu::loadMainMenu, this));
}

void Menu::loadEditorMenu() {
    choices.clear();
    actions.clear();
    selection = 0;

    std::vector<std::string> path_list = getResourceManager().getLevelList();
    std::vector<std::string> name_list;

    choices.push_back(L"Créer un nouveau niveau");
    actions.push_back(std::bind(&Menu::launchEditor, this, ""));

    for (auto it = path_list.begin(); it != path_list.end(); it++) {
        choices.push_back(Level::getLevelName(*it));
        actions.push_back(std::bind(&Menu::launchEditor, this, *it));
    }

    choices.push_back("Retour");
    actions.push_back(std::bind(&Menu::loadMainMenu, this));
}

void Menu::launchGame(std::string path) {
    auto game = std::unique_ptr<Game>(new Game(getManager()));
    game->load(path);
    getManager().pushState(std::move(game));
}

void Menu::launchRules() {
    auto rules = std::unique_ptr<Rules>(new Rules(getManager()));
    getManager().pushState(std::move(rules));
}

void Menu::launchEditor(std::string path) {
    auto editor = std::unique_ptr<Editor>(new Editor(getManager()));

    if (!path.empty()) {
        editor->load(path);
    }

    getManager().pushState(std::move(editor));
}

void Menu::quit() {
    getManager().popState();
}
