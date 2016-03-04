#include "engine.hpp"
#include <cmath>

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

        // réinitialise l'horloge à zéro
        // et stocke la différence de temps dans delta
        float delta = clock.restart().asSeconds();
        update(delta);
        draw();
    }
}

/**
 * Mise à jour de la physique du jeu
 */
void Engine::update(float delta) {
    for (int i = 0; i < balls.size(); i++) {
        sf::Vector2f forces(0, 0);

        // ajout de la force de gravité
        forces += sf::Vector2f(0, Engine::GRAVITY);

        // déplacement de la balle
        if (goLeftKey) {
            forces += sf::Vector2f(-Engine::MOVE, 0);
        }

        if (goRightKey) {
            forces += sf::Vector2f(Engine::MOVE, 0);
        }

        // force d'attraction
        for (int j = 0; j < balls.size(); j++) {
            if (i != j) {
                sf::Vector2f attraction(balls[i].getPosition() - balls[j].getPosition());
                float distanceSquared = attraction.x * attraction.x + attraction.y * attraction.y;

                attraction /= std::sqrt(distanceSquared);
                attraction *= Engine::ATTRACTION * ((balls[i].getCharge() * balls[j].getCharge()) / distanceSquared);

                forces += attraction;
            }
        }

        // TODO: COLLISIONS

        balls[i].update(forces, delta);
    }
}

/**
 * Dessine la scène du jeu couche par couche
 */
void Engine::draw() {
    // couche de fond
    window.clear(sf::Color(66, 165, 245));

    // grille de blocs
    for (int i = 0; i < blocks.size(); i++) {
        blocks[i].draw();
    }

    // dessin des balles
    for (int i = 0; i < balls.size(); i++) {
        balls[i].draw();
    }
}
