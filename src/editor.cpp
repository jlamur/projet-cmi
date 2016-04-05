#include <cmath>
#include "editor.hpp"
#include "block.hpp"
#include "constants.hpp"

Editor::Editor(Manager& manager) : Level(manager) {
    // activation de la synchronisation verticale
    // car, dans l'éditeur, nous n'avons besoin que de dessiner
    // (pas de mise à jour physique)
    manager.getWindow().setVerticalSyncEnabled(true);
}

Editor::~Editor() {}

void Editor::load(std::ifstream& file) {
    Level::load(file);
    manager.setTitle("Edition de " + getName());
}

void Editor::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();

    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];

        // lorsque l'on clique dans l'éditeur
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);

            if (event.mouseButton.button == sf::Mouse::Left) {
                // clic gauche : on met à jour la sélection,
                // si aucune mise à jour n'est à faire, on ajoute un objet
                if (!updateSelection(position)) {
                    addObject(position);
                }
            }

            if (event.mouseButton.button == sf::Mouse::Right) {
                // clic droit : on supprime l'objet pointé
                removeObject(position);
            }
        }
    }

    draw();
}

void Editor::draw() {
    Level::draw();
    sf::RenderWindow& window = manager.getWindow();
    sf::View window_view = manager.getWindowView();
    sf::Color selection_color(255, 50, 41);

    // on dessine des carrés de sélection autour des objets sélectionnés
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
    std::shared_ptr<Object> object = std::shared_ptr<Object>(new Block);
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
