#include "game.hpp"
#include "constants.hpp"
#include "object.hpp"
#include "block.hpp"
#include "player.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <queue>
#include <utility>
#include <iostream>

/**
 * Dictionnaire associant les types d'objets
 * à des instances qui seront utilisées pour la
 * construction d'autres objets de ces types
 */
std::map<unsigned int, std::function<std::shared_ptr<Object>(std::ifstream&)>> object_type_map = {
    {Player::TYPE_ID, Player::load},
    {Block::TYPE_ID, Block::load}
};

Game::Game(Manager& manager) : View(manager), accumulator(0.f) {}
Game::~Game() {
    objects.clear();
}

void Game::load(std::ifstream& file) {
    // vide le niveau précédent s'il y a lieu
    if (objects.size() != 0) {
        objects.clear();
    }

    // lecture de la signture du fichier ("BAR")
    char signature[3];
    file.read(signature, sizeof(signature));

    if (strncmp(signature, "BAR", sizeof(signature)) != 0) {
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

    // lecture de la zone de jeu
    char control_points;
    file.read(&control_points, 1);
    level_zone.clear();

    for (int i = 0; i < control_points; i++) {
        float pos_x, pos_y;

        file.read(reinterpret_cast<char*>(&pos_x), sizeof(pos_x));
        file.read(reinterpret_cast<char*>(&pos_y), sizeof(pos_y));

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        level_zone.push_back(std::make_pair(pos_x, pos_y));
    }

    // lecture des chemins de la musique et du fond
    std::string music_name, background_name;
    ResourceManager& resource_manager = manager.getResourceManager();

    std::getline(file, music_name, '\0');
    resource_manager.setMusic(music_name);
    resource_manager.playMusic();

    std::getline(file, background_name, '\0');
    background.setTexture(resource_manager.getTexture(background_name));

    // lecture du nombre de blocs
    int block_count;

    file.read(reinterpret_cast<char*>(&block_count), sizeof(block_count));
    block_count = ntohl(block_count);

    for (int i = 0; i < block_count; i++) {
        char block_type;
        file.read(&block_type, 1);

        // vérifie que le type est pris en charge
        // pour éviter une erreur de segmentation
        if (object_type_map.count(block_type) == 0) {
            throw std::runtime_error(
                "Impossible de lire le fichier : type d'objet inconnu"
            );
        }

        objects.push_back(object_type_map[block_type](file));
    }
}

void Game::save() {
    // TODO: faire une fonction d'enregistrement
    // TODO: migrer sur une classe commune Game <-> Editor
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
    sf::RenderWindow& window = manager.getWindow();
    window.clear(sf::Color(66, 165, 245));
    window.draw(background);

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

    window.display();
}
