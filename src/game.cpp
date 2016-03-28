#include "game.hpp"
#include "constants.hpp"
#include "player.hpp"
#include <cmath>
#include <queue>

Game::Game() : accumulator(0.f) {
    if (!music.openFromFile("./res/music_lvl1.wav")) {
        // erreur
    }

    music.play();
    music.setVolume(15);
    music.setLoop(true);

    load();
}

Game::~Game() {
    View::~View();
    clear();
}

void Game::load() {
    // vide le niveau précédent s'il y a lieu
    if (objects.size()) {
        clear();
    }

    // TODO: faire une vraie fonction de chargement
    Player* player1 = new Player(3.5f * Constants::GRID, 10 * Constants::GRID);
    objects.push_back(player1);
}

void Game::save() {
    // TODO: faire une fonction d'enregistrement
    // TODO: migrer sur une classe commune Game <-> Editor
}

void Game::clear() {
    for (unsigned int i = 0; i < objects.size(); i++) {
        delete objects[i];
    }

    objects.clear();
}

void Game::frame(Manager& manager) {
    accumulator += manager.getElapsedTime();

    // tant qu'il reste du temps à passer,
    // effectuer la simulation physique étape par étape
    while (accumulator >= Constants::PHYSICS_TIME) {
        accumulator -= Constants::PHYSICS_TIME;
        update(manager);
    }

    draw(manager);
}

void Game::update(const Manager& manager) {
    std::vector<CollisionData> colliding;

    // détection des objets en collision
    for (unsigned int i = 0; i < objects.size(); i++) {
        Object* objA = objects[i];

        for (unsigned int j = i + 1; j < objects.size(); j++) {
            Object* objB = objects[j];
            CollisionData data(*objA, *objB);

            if (objA->detectCollision(*objB, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse (première moitié)
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->updateVelocity(manager, objects, Constants::PHYSICS_TIME / 2);
    }

    // résolution des collisions détectées
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.solveCollision(collided.objB, collided.normal);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->updatePosition(Constants::PHYSICS_TIME);
    }

    // application de la correction positionnelle
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.positionalCorrection(
            collided.objB, collided.normal, collided.depth
        );
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < objects.size(); i++) {
        objects[i]->updateVelocity(manager, objects, Constants::PHYSICS_TIME / 2);
    }
}

void Game::draw(Manager& manager) {
    // efface la scène précédente et dessine la couche de fond
    manager.getWindow().clear(sf::Color(66, 165, 245));

    // chargement de la file d'affichage des objets
    std::priority_queue<Object*, std::vector<Object*>, ObjectCompare> display_queue;

    for (unsigned int i = 0; i < objects.size(); i++) {
        display_queue.push(objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    while (!display_queue.empty()) {
        display_queue.top()->draw(manager);
        display_queue.pop();
    }

    manager.getWindow().display();
}
