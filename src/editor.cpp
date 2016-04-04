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

void Editor::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();
    std::vector<ObjectPtr>& objects = getObjects();

    for (unsigned int i = 0; i < events.size(); i++) {
        const sf::Event& event = events[i];

        // lorsque l'on clique dans l'éditeur
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f position(event.mouseButton.x, event.mouseButton.y);
            int remove_object_index = -1;

            // recherche d'un objet intersectant la position cliquée
            for (unsigned int i = 0; i < objects.size(); i++) {
                if (objects[i]->getAABB()->contains(position)) {
                    remove_object_index = i;
                }
            }

            if (remove_object_index >= 0) {
                // si un objet a été cliqué, on le supprime
                objects.erase(objects.begin() + remove_object_index);
            } else {
                // sinon on crée un nouvel objet à la position cliquée
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
    }

    draw();
}
