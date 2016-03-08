#include "engine.hpp"
#include <cmath>
#include <queue>

/**
 * Fonction d'initialisation de la classe Engine
 */
Engine::Engine() {
    // initialise et ouvre la fenêtre
    window.create(
        sf::VideoMode(300, 150), "La cage de Jacob",
        sf::Style::Default & ~sf::Style::Resize,
        sf::ContextSettings(0, 0, 2)
    );

    window.setVerticalSyncEnabled(true);

    // au démarrage, aucune touche n'est enfoncée
    goLeftKey = false;
    goRightKey = false;

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

        // réinitialise l'horloge à zéro et calcule la différence
        // puis établit l'objet stockant l'état du moteur
        State state;
        state.delta = clock.restart().asSeconds();
        state.goLeftKey = goLeftKey;
        state.goRightKey = goRightKey;
        state.objects = objects;

        update(state);
        draw();
    }
}

void Engine::update(State state) {
    // demande la mise à jour de tous les objets du jeu
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->update(state, delta);
    }
}

void Engine::draw() {
    // efface la scène précédente et dessine la couche de fond
    window.clear(sf::Color(66, 165, 245));

    // chargement de la file d'affichage des objets
    typedef std::unique_ptr<Object> ObjectPtr;
    std::priority_queue<ObjectPtr, std::vector<ObjectPtr>, CompareObjectLayer> display_queue;

    for (int i = 0; i < objects.size(); i++) {
        display_queue.push(objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    unsigned int layer = 0;

    while (!display_queue.empty()) {
        display_queue.pop()->draw(window);
    }
}
