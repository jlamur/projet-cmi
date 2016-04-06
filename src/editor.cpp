#include <cmath>
#include <iostream>
#include "editor.hpp"
#include "game.hpp"
#include "block.hpp"
#include "constants.hpp"

Editor::Editor(Manager& manager) : Level(manager),
    widget_timer(manager, std::bind(&Editor::setTotalTime, this, std::placeholders::_1)) {}

Editor::~Editor() {}

void Editor::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle(sf::String(L"Édition de ") + getName());
}

void Editor::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();

    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];

        // traitement des événements du widget timer
        if (widget_timer.processEvent(event)) {
            continue;
        }

        // lorsque l'on clique dans l'éditeur
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

            if (event.mouseButton.button == sf::Mouse::Left) {
                // clic gauche : on met à jour la sélection,
                // si aucune mise à jour n'est à faire, on ajoute un objet
                if (!updateSelection(position)) {
                    addObject(position);
                }
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                // clic droit : on supprime l'objet pointé
                removeObject(position);
            }
        }

        // gestion des touches
        if (event.type == sf::Event::KeyPressed) {
            // appui sur espace : test du niveau en cours d'édition
            if (event.key.code == sf::Keyboard::Space) {
                testLevel();
                return; // important : ne pas dessiner la frame
                // on risque d'avoir perdu le pointeur en changeant de vue
            }
        }
    }

    // dessin de la frame
    draw();
    sf::sleep(sf::seconds(1.f / 60));
}

void Editor::draw() {
    Level::draw();

    sf::RenderWindow& window = manager.getWindow();
    sf::View window_view = manager.getWindowView();
    sf::Color selection_color(255, 50, 41);

    // dessin de la sélection autour des objets sélectionnés
    for (auto iter = selection.begin(); iter != selection.end(); iter++) {
        sf::VertexArray selection(sf::LinesStrip, 5);
        std::unique_ptr<sf::FloatRect> aabb = iter->first->getAABB();

        selection[0].position = sf::Vector2f(aabb->left - .5f, aabb->top - .5f);
        selection[0].color = selection_color;
        selection[1].position = sf::Vector2f(aabb->left + aabb->width + .5f, aabb->top - .5f);
        selection[1].color = selection_color;
        selection[2].position = sf::Vector2f(aabb->left + aabb->width + .5f, aabb->top + aabb->height + .5f);
        selection[2].color = selection_color;
        selection[3].position = sf::Vector2f(aabb->left - .5f, aabb->top + aabb->height + .5f);
        selection[3].color = selection_color;
        selection[4].position = sf::Vector2f(aabb->left - .5f, aabb->top - .5f);
        selection[4].color = selection_color;

        window.draw(selection);
    }

    // dessin du widget timer
    widget_timer.setTimeLeft(getTotalTime());
    widget_timer.draw(sf::Vector2f(window_view.getSize().x / 2 - 50, 0));

    // menu
    sf::RectangleShape menu(sf::Vector2f(window_view.getSize().x, 64));
    menu.setPosition(sf::Vector2f(0, window_view.getSize().y - 64));

    window.draw(menu);
}

void Editor::addObject(sf::Vector2f position) {
    std::vector<ObjectPtr>& objects = getObjects();

    // si demandé, on arrondit à l'unité de grille la plus proche
    position /= Constants::GRID;
    position.x = round(position.x);
    position.y = round(position.y);
    position *= Constants::GRID;

    // TODO: ajouter un objet du type choisi, pas uniquement de bloc
    ObjectPtr object = ObjectPtr(new Block);
    object->setPosition(position);

    // avant d'ajouter l'objet, on vérifie qu'il ne soit
    // pas superposé à un autre
    float overlaps = false;

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->intersects(*object->getAABB())) {
            overlaps = true;
        }
    }

    if (!overlaps) {
        objects.push_back(object);
        updateSelection(position);
    }
}

void Editor::removeObject(sf::Vector2f position) {
    std::vector<ObjectPtr>& objects = getObjects();
    int remove_object_index = -1;

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->contains(position)) {
            remove_object_index = i;
        }
    }

    if (remove_object_index >= 0) {
        selection.erase(objects[remove_object_index]);
        objects.erase(objects.begin() + remove_object_index);
    }
}

bool Editor::updateSelection(sf::Vector2f position) {
    std::vector<ObjectPtr>& objects = getObjects();
    bool has_changed = false;
    bool multi = manager.isKeyPressed(sf::Keyboard::LShift);

    for (unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->getAABB()->contains(position)) {
            has_changed = true;

            // si l'objet n'est pas sélectionné, on le sélectionne
            // sinon on le désélectionne
            if (selection.count(objects[i])) {
                selection.erase(objects[i]);
            } else {
                // avant de sélectionner le nouvel objet, on
                // vide la sélection si on n'est pas en mode multi
                if (!multi) {
                    selection.clear();
                }

                selection[objects[i]] = true;
            }
        }
    }

    return has_changed;
}

void Editor::testLevel() {
    std::shared_ptr<Game> game = std::shared_ptr<Game>(new Game(manager));

    // copie des propriétés
    game->setName(getName());
    game->setTotalTime(getTotalTime());
    game->setBackground(getBackground());

    // copie des objets du niveau vers le jeu
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < objects.size(); i++) {
        game->getObjects().push_back(objects[i]->clone());
    }

    // copie de la zone de jeu
    std::vector<std::pair<float, float>>& zone = getZone();

    for (unsigned int i = 0; i < zone.size(); i++) {
        game->getZone().push_back(zone[i]);
    }

    // mise en mode test
    game->setTestMode(manager.getView());
    manager.setView(game);
}
