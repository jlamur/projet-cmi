#include <cmath>
#include <iostream>
#include "editor.hpp"
#include "block.hpp"
#include "constants.hpp"

Editor::Editor(Manager& manager) : Level(manager), selected_object(nullptr) {
    // activation de la synchronisation verticale
    // car, dans l'éditeur, nous n'avons besoin que de dessiner
    // (pas de mise à jour physique)
    manager.getWindow().setVerticalSyncEnabled(true);
}

Editor::~Editor() {}

void Editor::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];

        // lorsque l'on clique dans l'éditeur
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
            bool selection_changed = false;

            if (event.mouseButton.button == sf::Mouse::Left) {
                // recherche d'un objet intersectant la position cliquée
                // et le sélectionne si c'est le cas
                for (unsigned int i = 0; i < objects.size(); i++) {
                    if (objects[i]->getAABB()->contains(position)) {
                        selection_changed = true;

                        // si l'objet n'est pas sélectionné, on le sélectionne
                        // sinon on le désélectionne
                        if (objects[i]->isSelected()) {
                            objects[i]->setSelected(false);
                            selected_object = nullptr;
                        } else {
                            // désélection du précédent objet (si applicable)
                            if (selected_object != nullptr) {
                                selected_object->setSelected(false);
                            }

                            selected_object = objects[i];
                            objects[i]->setSelected(true);
                        }
                    }
                }

                // si aucune opération de sélection, on
                // crée un nouvel objet à la position cliquée
                if (!selection_changed) {
                    // on désélectionne tout objet sélectionné
                    if (selected_object != nullptr) {
                        selected_object->setSelected(false);
                        selected_object = nullptr;
                    }

                    position /= Constants::GRID;
                    position.x = round(position.x);
                    position.y = round(position.y);
                    position *= Constants::GRID;

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
                    }
                }
            }

            if (event.mouseButton.button == sf::Mouse::Right) {
                int remove_object_index = -1;

                for (unsigned int i = 0; i < objects.size(); i++) {
                    if (objects[i]->getAABB()->contains(position)) {
                        remove_object_index = i;
                    }
                }

                if (remove_object_index >= 0) {
                    objects.erase(objects.begin() + remove_object_index);
                }
            }
        }
    }

    draw();
}

void Editor::draw() {
    Level::draw();

    // menu
    sf::RectangleShape menu(sf::Vector2f(manager.getWindowView().getSize().x, 64));
    menu.setPosition(sf::Vector2f(0, manager.getWindowView().getSize().y - 64));

    manager.getWindow().draw(menu);
}
