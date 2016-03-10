#include "engine.hpp"
#include "state.hpp"
#include <cmath>
#include <queue>

Engine::Engine() : window(
    sf::VideoMode(300, 150), "Projet CMI",
    sf::Style::Default & ~sf::Style::Resize,
    sf::ContextSettings(0, 0, 2)
), goLeftKey(false), goRightKey(false) {
    window.setVerticalSyncEnabled(true);
}

void Engine::start() {
    // boucle d'événements sur la fenêtre
    while (window.isOpen()) {
        sf::Event event;

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // une touche a été enfoncée
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    goLeftKey = true;
                }

                if (event.key.code == sf::Keyboard::Right) {
                    goRightKey = true;
                }
            }

            // une touche a été relâchée
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Left) {
                    goLeftKey = false;
                }

                if (event.key.code == sf::Keyboard::Right) {
                    goRightKey = false;
                }
            }
        }

        update();
        draw();
    }
}

void Engine::addObject(Object* object) {
    objects.push_back(object);
}

void Engine::update() {
    // calcul du temps écoulé depuis la dernière frame
    // et création de l'objet state stockant l'état du moteur
    State state;
    state.delta = clock.restart().asSeconds();
    state.goLeftKey = goLeftKey;
    state.goRightKey = goRightKey;
    state.objects = objects;

    // demande la mise à jour de tous les objets du jeu
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->update(state);
    }
}

void Engine::draw() {
    // efface la scène précédente et dessine la couche de fond
    window.clear(sf::Color(66, 165, 245));

    // chargement de la file d'affichage des objets
    std::priority_queue<Object*, std::vector<Object*>, CompareObjectLayer> display_queue;

    for (unsigned int i = 0; i < objects.size(); i++) {
        display_queue.push(objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    while (!display_queue.empty()) {
        display_queue.top()->draw(window);
        display_queue.pop();
    }
}
