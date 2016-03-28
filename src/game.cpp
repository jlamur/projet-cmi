#include "game.hpp"
#include "constants.hpp"
#include <cmath>
#include <queue>

Game::Game() : window(
    sf::VideoMode(704, 480), "Projet CMI", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), view(sf::FloatRect(0, 0, 704, 480)) {
    window.setVerticalSyncEnabled(true);

    // mise en place de la caméra
    view.setSize(704, 480);
    window.setView(view);
}

void Game::start() {
    float accumulator = 0;
    if (!music.openFromFile("./res/music_lvl1.wav"))
    {
        // erreur
    }
    music.play();
    music.setVolume(15);
    music.setLoop(true);

    // boucle d'événements sur la fenêtre
    while (window.isOpen()) {
        sf::Event event;

        // traitement des évènements reçus
        while (window.pollEvent(event)) {
            // fermeture de la fenêtre
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // suivi de l'enfoncement et du relâchement des touches
            if (event.type == sf::Event::KeyPressed) {
                state.keys[event.key.code] = true;
            }

            if (event.type == sf::Event::KeyReleased) {
                state.keys[event.key.code] = false;
            }

			// lorsque la fenêtre est redimensionnée par l'utilisateur,
            // mise à jour de la caméra en fonction de la taille de la fenêtre
    		if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        float frame = clock.restart().asSeconds();
        accumulator += frame;

        // tant qu'il reste du temps à passer,
        // effectuer la simulation physique étape par étape
        while (accumulator >= Constants::PHYSICS_TIME) {
            accumulator -= Constants::PHYSICS_TIME;
            update();
        }

        draw();
    }
}

void Game::addObject(Object& object) {
    state.objects.push_back(&object);
}

void Game::update() {
    std::vector<CollisionData> colliding;

    // détection des objets en collision
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        Object* objA = state.objects[i];

        for (unsigned int j = i + 1; j < state.objects.size(); j++) {
            Object* objB = state.objects[j];
            CollisionData data(*objA, *objB);

            if (objA->detectCollision(*objB, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse (première moitié)
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updateVelocity(state, Constants::PHYSICS_TIME / 2);
    }

    // résolution des collisions détectées
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.solveCollision(collided.objB, collided.normal);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updatePosition(state, Constants::PHYSICS_TIME);
    }

    // application de la correction positionnelle
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.positionalCorrection(
            collided.objB, collided.normal, collided.depth
        );
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updateVelocity(state, Constants::PHYSICS_TIME / 2);
    }
}

void Game::draw() {
    // efface la scène précédente et dessine la couche de fond
    window.clear(sf::Color(66, 165, 245));

    // chargement de la file d'affichage des objets
    std::priority_queue<Object*, std::vector<Object*>, ObjectCompare> display_queue;

    for (unsigned int i = 0; i < state.objects.size(); i++) {
        display_queue.push(state.objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    while (!display_queue.empty()) {
        display_queue.top()->draw(window, resources);
        display_queue.pop();
    }

    window.display();
}
