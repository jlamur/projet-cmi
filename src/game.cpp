#include "game.hpp"
#include "constants.hpp"
#include "player.hpp"
#include <cstring>
#include <queue>
#include <utility>
#include <iostream>

Game::Game(Manager& manager) : View(manager), accumulator(0.f) {}
Game::~Game() {
    clear();
}

void Game::load(std::ifstream& file) {
    // vide le niveau précédent s'il y a lieu
    if (objects.size()) {
        clear();
    }

    // lecture de la signture du fichier ("BAR")
    char signature[3];
    file.read(signature, sizeof(signature));

    if (strncmp(signature, "BAR", 3) != 0) {
        throw std::runtime_error(
            "Impossible de lire le fichier : en-tête invalide"
        );
    }

    // lecture de la version du fichier
    char file_version;
    file.read(&file_version, 1);

    if (file_version != 0) {
        throw std::runtime_error(
            "Impossible de lire le fichier : version non prise en charge"
        );
    }

    // lecture du nom du niveau
    std::getline(file, level_name, '\0');

    // lecture des positions initiales
    char player_amount;
    file.read(&player_amount, 1);

    for (int i = 0; i < player_amount; i++) {
        float pos_x, pos_y;

        file.read((char*) &pos_x, sizeof(pos_x));
        file.read((char*) &pos_y, sizeof(pos_y));

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        std::shared_ptr<Player> player = std::make_shared<Player>(pos_x, pos_y);
        player->setPlayerNumber(i);

        objects.push_back(std::dynamic_pointer_cast<Object>(player));
    }

    // lecture de la zone de vie
    char control_points;
    file.read(&control_points, 1);
    level_zone.clear();

    for (int i = 0; i < control_points; i++) {
        float pos_x, pos_y;

        file.read((char*) &pos_x, sizeof(pos_x));
        file.read((char*) &pos_y, sizeof(pos_y));

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        level_zone.push_back(std::make_pair(pos_x, pos_y));
    }

    // lecture des chemins de la musique et du fond
    std::getline(file, music_path, '\0');
    std::getline(file, background_path, '\0');

    manager.getResourceManager().setMusic(music_path);
    manager.getResourceManager().playMusic();
}

void Game::save() {
    // TODO: faire une fonction d'enregistrement
    // TODO: migrer sur une classe commune Game <-> Editor
}

void Game::clear() {
    objects.clear();
}

void Game::frame() {
    accumulator += manager.getElapsedTime();

    // tant qu'il reste du temps à passer,
    // effectuer la simulation physique étape par étape
    while (accumulator >= Constants::PHYSICS_TIME) {
        accumulator -= Constants::PHYSICS_TIME;
        update();
    }

    draw();
}

void Game::update() {
    std::vector<CollisionData> colliding;

    // détection des objets en collision
    for (unsigned int i = 0; i < objects.size(); i++) {
        ObjectPtr objA = objects[i];

        for (unsigned int j = i + 1; j < objects.size(); j++) {
            ObjectPtr objB = objects[j];
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

void Game::draw() {
    // efface la scène précédente et dessine la couche de fond
    manager.getWindow().clear(sf::Color(66, 165, 245));

    // chargement de la file d'affichage des objets
    std::priority_queue<ObjectPtr, std::vector<ObjectPtr>, ObjectCompare> display_queue;

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
