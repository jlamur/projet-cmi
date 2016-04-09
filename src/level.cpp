#include "constants.hpp"
#include "level.hpp"
#include "player.hpp"
#include "block.hpp"
#include "gravity_block.hpp"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <queue>
#include <utility>

/**
 * Constante de gravité
 */
const float GRAVITY = 235;

/**
 * Constante de déplacement des objets à déplacement manuel
 */
const float MOVE = 200;

/**
 * Dictionnaire associant les types d'objets
 * à des instances qui seront utilisées pour la
 * construction d'autres objets de ces types
 */
std::map<unsigned int, std::function<ObjectPtr(std::ifstream&)>> object_type_map = {
    {Player::TYPE_ID, Player::load},
    {Block::TYPE_ID, Block::load},
    {GravityBlock::TYPE_ID, GravityBlock::load}
};

Level::Level(Manager& manager) : State(manager), camera_angle(180.f),
    gravity_direction(GravityDirection::SOUTH) {}
Level::~Level() {}

void Level::load(std::string name) {
    LevelReader file = getResourceManager().getLevelReader(name);

    // vidage du niveau précédent et positionnement
    // de la caméra au centre du niveau
    objects.clear();
    camera.setCenter(0, 0);

    // lecture de la signture du fichier ("BAR")
    char signature[3];
    file->read(signature, sizeof(signature));

    if (strncmp(signature, "BAR", sizeof(signature)) != 0) {
        throw std::runtime_error(
            "Impossible de lire le fichier : en-tête invalide"
        );
    }

    // lecture de la version du fichier
    char file_version;
    file->read(&file_version, 1);

    if (file_version != 0) {
        throw std::runtime_error(
            "Impossible de lire le fichier : version non prise en charge"
        );
    }

    // lecture du nom du niveau
    std::string std_name;
    std::getline(*file, std_name, '\0');
    name = sf::String(std_name);

    // lecture du temps total du niveau
    file->read(reinterpret_cast<char*>(&total_time), sizeof(total_time));
    total_time = ntohl(total_time);

    // lecture de la zone de jeu
    char control_points;
    file->read(&control_points, 1);
    zone.clear();

    for (int i = 0; i < control_points; i++) {
        float pos_x, pos_y;

        file->read(reinterpret_cast<char*>(&pos_x), sizeof(pos_x));
        file->read(reinterpret_cast<char*>(&pos_y), sizeof(pos_y));

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        zone.push_back(std::make_pair(pos_x, pos_y));
    }

    // lecture des chemins de la musique et du fond
    std::string background_name;

    std::getline(*file, music_name, '\0');
    std::getline(*file, background_name, '\0');
    background.setTexture(getResourceManager().getTexture(background_name));

    // lecture du nombre de blocs
    int block_count;

    file->read(reinterpret_cast<char*>(&block_count), sizeof(block_count));
    block_count = ntohl(block_count);

    for (int i = 0; i < block_count; i++) {
        char block_type;
        file->read(&block_type, 1);

        // vérifie que le type est pris en charge
        // pour éviter une erreur de segmentation
        if (object_type_map.count(block_type) == 0) {
            throw std::runtime_error(
                "Impossible de lire le fichier : type d'objet inconnu"
            );
        }

        objects.push_back(object_type_map[block_type](*file));
    }

    file->close();
}

void Level::save(std::string name) {
    LevelWriter file = getResourceManager().getLevelWriter(name);

    // TODO: faire une fonction d'enregistrement

    file->close();
}

void Level::begin() {
    camera = getWindow().getDefaultView();
    camera.setCenter(0, 0);

    if (music_name != "") {
        getResourceManager().playMusic(music_name);
    } else {
        getResourceManager().stopMusic();
    }
}

void Level::processEvent(const sf::Event& event) {
    // lorsque la fenêtre est redimensionnée, mise à jour
    // de la taille de la caméra
    if (event.type == sf::Event::Resized) {
        camera.setSize(event.size.width, event.size.height);
    }
}

void Level::draw() {
    sf::RenderWindow& window = getWindow();

    // animation de la rotation de la caméra
    float goal = std::fmod((float) gravity_direction * 90, 360);
    float diff = goal - camera_angle;
    float speed = diff * Constants::PHYSICS_TIME.asSeconds() * 5;

    if (std::abs(diff) < .05f) {
        camera_angle = goal;
    } else {
        camera_angle += speed;
    }

    camera.setRotation(camera_angle + 180);
    window.setView(camera);

    // efface la scène précédente et dessine la couche de fond
    window.clear(sf::Color(66, 165, 245));
    window.draw(background);

    // chargement de la file d'affichage des objets
    std::priority_queue<ObjectPtr, std::vector<ObjectPtr>, ObjectCompare> display_queue;

    for (unsigned int i = 0; i < objects.size(); i++) {
        display_queue.push(objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    while (!display_queue.empty()) {
        display_queue.top()->draw(*this);
        display_queue.pop();
    }

    // passage sur la vue par défaut
    getManager().resetDefaultView();
}

sf::String Level::getName() const {
    return name;
}

void Level::setName(sf::String set_name) {
    name = set_name;
}

int Level::getTotalTime() const {
    return total_time;
}

void Level::setTotalTime(int set_total_time) {
    // faisons rester le temps entre 10s et 59:59
    set_total_time = std::min(set_total_time, 3599);
    set_total_time = std::max(set_total_time, 10);

    total_time = set_total_time;
}

sf::Sprite Level::getBackground() const {
    return background;
}

void Level::setBackground(sf::Sprite set_background) {
    background = set_background;
}

sf::Vector2f Level::getGravity() const {
    switch (gravity_direction) {
    case GravityDirection::NORTH:
        return sf::Vector2f(0, -GRAVITY);

    case GravityDirection::EAST:
        return sf::Vector2f(GRAVITY, 0);

    case GravityDirection::SOUTH:
        return sf::Vector2f(0, GRAVITY);

    case GravityDirection::WEST:
        return sf::Vector2f(-GRAVITY, 0);
    }

    return sf::Vector2f(0, 0);
}

sf::Vector2f Level::getLeftDirection() const {
    switch (gravity_direction) {
    case GravityDirection::NORTH:
        return sf::Vector2f(MOVE, 0);

    case GravityDirection::EAST:
        return sf::Vector2f(0, MOVE);

    case GravityDirection::SOUTH:
        return sf::Vector2f(-MOVE, 0);

    case GravityDirection::WEST:
        return sf::Vector2f(0, -MOVE);
    }

    return sf::Vector2f(0, 0);
}

sf::Vector2f Level::getRightDirection() const {
    return -1.f * getLeftDirection();
}

GravityDirection Level::getGravityDirection() {
    return gravity_direction;
}

void Level::setGravityDirection(GravityDirection set_gravity_direction) {
    gravity_direction = set_gravity_direction;
}

std::vector<ObjectPtr>& Level::getObjects() {
    return objects;
}

const std::vector<ObjectPtr>& Level::getObjects() const {
    return objects;
}

std::vector<std::pair<float, float>>& Level::getZone() {
    return zone;
}

const std::vector<std::pair<float, float>>& Level::getZone() const {
    return zone;
}

sf::View Level::getCamera() const {
    return camera;
}

sf::Vector2f Level::pixelToCoords(sf::Vector2i pixel) {
    sf::RenderWindow& window = getWindow();
    sf::View old_view = window.getView();

    window.setView(camera);
    sf::Vector2f coords = window.mapPixelToCoords(pixel);
    window.setView(old_view);

    return coords;
}

sf::Vector2i Level::coordsToPixel(sf::Vector2f coords) {
    sf::RenderWindow& window = getWindow();
    sf::View old_view = window.getView();

    window.setView(camera);
    sf::Vector2i pixel = window.mapCoordsToPixel(coords);
    window.setView(old_view);

    return pixel;
}

void Level::setCamera(sf::View set_camera) {
    camera = set_camera;
}
