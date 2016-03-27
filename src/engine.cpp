#include "engine.hpp"
#include "constants.hpp"
#include <cmath>
#include <queue>

Engine::Engine() : window(
    sf::VideoMode(704, 480), "Projet CMI", sf::Style::Default,
    sf::ContextSettings(0, 0, 2)
), view(sf::FloatRect(0, 0, 704, 480)){
    window.setVerticalSyncEnabled(true);

    // mise en place de la caméra
    view.setSize(704, 480);
    window.setView(view);
}

void Engine::start() {
    float accumulator = 0;

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

void Engine::addObject(Object& object) {
    state.objects.push_back(&object);
}

void Engine::update() {
    // gestion des collisions entre les objets
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        Object* objA = state.objects[i];

        for (unsigned int j = i + 1; j < state.objects.size(); j++) {
            Object* objB = state.objects[j];
            objA->collide(*objB);
        }
    }

    // intégration des forces dans la vitesse (première moitié)
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updateVelocity(state, Constants::PHYSICS_TIME / 2);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updatePosition(state, Constants::PHYSICS_TIME);
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < state.objects.size(); i++) {
        state.objects[i]->updateVelocity(state, Constants::PHYSICS_TIME / 2);
    }
}

void Engine::draw() {
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
