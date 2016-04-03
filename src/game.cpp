#include "game.hpp"
#include "constants.hpp"

Game::Game(Manager& manager) : Level(manager),
    next_frame_time(manager.getCurrentTime()) {}
Game::~Game() {}

void Game::frame() {
    sf::Time current_time = manager.getCurrentTime();

    if (current_time >= next_frame_time) {
        // si nous sommes en retard ou dans les temps
        // on replanifie la prochaine frame
        next_frame_time += Constants::PHYSICS_TIME;

        // on met à jour la physique d'un cran temporel
        update();

        // si on a encore suffisamment de temps, on dessine
        if (current_time < next_frame_time) {
            draw();
        }
    } else {
        // si nous sommes en avance, on endort le processus
        // le temps nécessaire pour revenir dans les temps
        sf::sleep(next_frame_time - current_time);
    }
}

void Game::update() {
    std::vector<CollisionData> colliding;

    // détection des objets en collision
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        ObjectPtr objA = getObjects()[i];

        for (unsigned int j = i + 1; j < getObjects().size(); j++) {
            ObjectPtr objB = getObjects()[j];
            CollisionData data(*objA, *objB);

            if (objA->detectCollision(*objB, data)) {
                colliding.push_back(data);
            }
        }
    }

    // intégration des forces dans la vitesse (première moitié)
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updateVelocity(manager, getObjects(), Constants::PHYSICS_TIME.asSeconds() / 2);
    }

    // résolution des collisions détectées
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.solveCollision(collided.objB, collided.normal);
    }

    // intégration de la vitesse dans la position
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updatePosition(Constants::PHYSICS_TIME.asSeconds());
    }

    // application de la correction positionnelle
    for (unsigned int i = 0; i < colliding.size(); i++) {
        CollisionData& collided = colliding[i];
        collided.objA.positionalCorrection(
            collided.objB, collided.normal, collided.depth
        );
    }

    // intégration des forces dans la vitesse (seconde moitié)
    for (unsigned int i = 0; i < getObjects().size(); i++) {
        getObjects()[i]->updateVelocity(manager, getObjects(), Constants::PHYSICS_TIME.asSeconds() / 2);
    }
}
