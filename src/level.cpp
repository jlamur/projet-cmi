#include "constants.hpp"
#include "level.hpp"
#include "player.hpp"
#include "block.hpp"
#include "gravity_block.hpp"
#include "switch_block.hpp"
#include "finish_block.hpp"
#include "kill_block.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <queue>

/**
 * Constante de gravité
 */
const float GRAVITY = 235;

/**
 * Constante de déplacement des objets à déplacement manuel
 */
const float MOVE = 200;

/**
 * Numéro actuel de version du format de fichier
 */
const unsigned int VERSION_NUMBER = 0;

/**
 * Dictionnaire associant les types d'objets
 * à des instances qui seront utilisées pour la
 * construction d'autres objets de ces types
 */
std::map<unsigned int, std::function<Object::Ptr(std::ifstream&)>> object_type_map = {
    {Player::TYPE_ID, Player::load},
    {Block::TYPE_ID, Block::load},
    {GravityBlock::TYPE_ID, GravityBlock::load},
    {FinishBlock::TYPE_ID, FinishBlock::load},
    {KillBlock::TYPE_ID, KillBlock::load},
    {SwitchBlock::TYPE_ID, SwitchBlock::load}
};

/**
 * Lecture du niveau dont le chemin absolu complet est en paramètre.
 * Les métadonnées sont stockées dans les variables passées par référence.
 * La callback object() est appelée séquentiellement avec les objets du niveau
 */
void loadLevel(
    std::string path, sf::String& name, int& total_time,
    std::vector<sf::Vector2f>& zone,
    std::string& background, std::string& music,
    std::function<Object::Ptr(Object::Ptr)> object_callback = {}
) {
    // ouverture du fichier
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    // on vérifie que le fichier ait correctement été ouvert en lecture
    if (file.fail()) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + name + "\" " +
            "(" + std::string(strerror(errno)) + ")"
        );
    }

    // lecture de la signature du fichier ("BAR")
    char signature[3];
    file.read(signature, 3);

    if (strncmp(signature, "BAR", 3) != 0) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + name + "\" " +
            "(en-tête invalide)"
        );
    }

    // lecture de la version du fichier
    char file_version;
    file.read(&file_version, 1);

    if (file_version != VERSION_NUMBER) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + name + "\" " +
            "(version non prise en charge)"
        );
    }

    // lecture du nom du niveau
    std::string std_name;
    std::getline(file, std_name, '\0');
    name = sf::String(std_name);

    // lecture du temps total du niveau
    file.read(reinterpret_cast<char*>(&total_time), 4);
    total_time = ntohl(total_time);

    // lecture de la zone de jeu
    char control_points;
    file.read(&control_points, 1);
    zone.clear();

    for (int i = 0; i < control_points; i++) {
        float pos_x, pos_y;

        file.read(reinterpret_cast<char*>(&pos_x), 4);
        file.read(reinterpret_cast<char*>(&pos_y), 4);

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        zone.push_back(sf::Vector2f(pos_x, pos_y));
    }

    // lecture des chemins de la musique et du fond
    std::getline(file, music, '\0');
    std::getline(file, background, '\0');

    // lecture des objets si une callback a été fournie
    int object_count;

    if (!object_callback) {
        return;
    }

    file.read(reinterpret_cast<char*>(&object_count), 4);
    object_count = ntohl(object_count);

    for (int i = 0; i < object_count; i++) {
        char object_type;
        file.read(&object_type, 1);

        // vérifie que le type est pris en charge
        // pour éviter une erreur de segmentation
        if (object_type_map.count(object_type) == 0) {
            throw std::runtime_error(
                "Impossible de charger le niveau \"" + name + "\" " +
                "(type d'objet " + std::to_string(object_type) + " inconnu)"
            );
        }

        // lecture de l'objet
        object_callback(object_type_map[object_type](file));
    }
}

Level::Level(Manager& manager) : State(manager) {
    sf::Vector2u window_size = getWindow().getSize();
    gravity_direction = GravityDirection::SOUTH;

    camera.setSize(window_size.x, window_size.y);
    camera.setCenter(0, 0);
    camera_angle = 180.f;
}

Level::~Level() {}

sf::String Level::getLevelName(std::string path) {
    sf::String name;
    int total_time;
    std::vector<sf::Vector2f> zone;
    std::string background;
    std::string music;

    loadLevel(path, name, total_time, zone, background, music);
    return name;
}

void Level::load() {
    // métadonnées par défaut
    name = sf::String("Nouveau niveau");
    time_left = total_time = 30;

    // zone de jeu par défaut
    zone.clear();
    zone.push_back(sf::Vector2f(-128, -128));
    zone.push_back(sf::Vector2f(128, -128));
    zone.push_back(sf::Vector2f(128, 128));
    zone.push_back(sf::Vector2f(-128, 128));

    // ressources par défaut
    music = "";
    background = "";

    // objets par défaut
    // TODO: ajouter quelques objets par défaut
}

void Level::load(std::string path) {
    // si le fichier n'existe pas, on utilise le niveau par défaut
    if (!boost::filesystem::exists(path)) {
        load();
        current_path = path;
        return;
    }

    loadLevel(
        path, name, total_time,
        zone, background, music,
        std::bind(&Level::addObject, this, std::placeholders::_1)
    );

    time_left = total_time;
    current_path = path;
}

void Level::save(std::string path) {
    std::ofstream file;
    file.open(path, std::ios::binary | std::ios::out);

    // on vérifie que le fichier ait correctement été ouvert en lecture
    if (file.fail()) {
        throw std::runtime_error(
            "Impossible d'enregistrer le niveau \"" + name + "\" " +
            "(" + std::string(strerror(errno)) + ")"
        );
    }

    // écriture de la signture du fichier ("BAR")
    char signature[3] = {'B', 'A', 'R'};
    file.write(signature, 3);

    // écriture de la version du fichier
    char file_version = VERSION_NUMBER;
    file.write(&file_version, 1);

    // écriture du nom du niveau
    char *write_name = (char*) name.toAnsiString().data();
    file.write(write_name, name.getSize() + 1);

    // écriture du temps total du niveau
    int conv_total_time = htonl(total_time);
    file.write(reinterpret_cast<char*>(&conv_total_time), 4);

    // écriture de la zone de jeu
    char control_points = (char) zone.size();
    file.write(&control_points, 1);

    for (int i = 0; i < control_points; i++) {
        float pos_x = zone[i].x / Constants::GRID;
        float pos_y = zone[i].y / Constants::GRID;

        file.write(reinterpret_cast<char*>(&pos_x), 4);
        file.write(reinterpret_cast<char*>(&pos_y), 4);
    }

    // écriture des noms de la musique et du fond
    char *write_music = (char*) music.data();
    file.write(write_music, music.size() + 1);

    char *write_background = (char*) background.data();
    file.write(write_background, background.size() + 1);

    // écriture des objets
    int object_count = htonl(objects.size());
    file.write(reinterpret_cast<char*>(&object_count), 4);

    for (unsigned int i = 0; i < objects.size(); i++) {
        char object_type = objects[i]->getTypeId();
        file.write(&object_type, 1);

        // écriture de l'objet
        objects[i]->save(file);
    }

    std::cout << "Sauvegardé : " << path << std::endl;
}

void Level::save() {
    save(current_path);
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
    float speed = diff * Manager::FRAME_TIME.asSeconds() * 5;

    if (std::abs(diff) < .05f) {
        camera_angle = goal;
    } else {
        camera_angle += speed;
    }

    camera.setRotation(camera_angle + 180);
    window.setView(camera);

    // efface la scène précédente et dessine la couche de fond
    window.clear(sf::Color(66, 165, 245));

    if (background != "") {
        background_sprite.setTexture(getResourceManager().getTexture(background));
    }

    window.draw(background_sprite);

    // chargement de la file d'affichage des objets
    std::priority_queue<Object::Ptr, std::vector<Object::Ptr>, ObjectCompare> display_queue;

    for (unsigned int i = 0; i < objects.size(); i++) {
        display_queue.push(objects[i]);
    }

    // dessin des objets de la file d'affichage couche par couche
    while (!display_queue.empty()) {
        display_queue.top()->draw(*this);
        display_queue.pop();
    }
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
    time_left = total_time;
}

std::string Level::getMusic() const {
    return music;
}

void Level::setMusic(std::string set_music) {
    music = set_music;
}

std::string Level::getBackground() const {
    return background;
}

void Level::setBackground(std::string set_background) {
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

std::vector<Object::Ptr>& Level::getObjects()  {
    return objects;
}

const std::vector<Object::Ptr>& Level::getObjects() const {
    return objects;
}

std::vector<Player::Ptr>& Level::getPlayers() {
    return players;
}

const std::vector<Player::Ptr>& Level::getPlayers() const {
    return players;
}

Object::Ptr Level::addObject(Object::Ptr object) {
    // si c'est un joueur, on le met dans le tableau des joueurs
    // et on lui attribue un numéro
    if (object->getTypeId() == Player::TYPE_ID) {
        // on n'autorise pas plus de deux joueurs
        if (players.size() >= 2) {
            return nullptr;
        }

        Player::Ptr player = std::dynamic_pointer_cast<Player>(object);

        player->setPlayerNumber(players.size());
        players.push_back(std::dynamic_pointer_cast<Player>(object));
    }

    objects.push_back(object);
    return object;
}

void Level::removeObject(Object::Ptr object) {
    // si c'est un joueur, on le supprime de la liste des joueurs
    if (object->getTypeId() == Player::TYPE_ID) {
        Player::Ptr player = std::dynamic_pointer_cast<Player>(object);
        players.erase(std::remove(
            players.begin(), players.end(), player
        ), players.end());

        // on réattribue les numéros de joueurs
        for (unsigned int i = 0; i < players.size(); i++) {
            players[i]->setPlayerNumber(i);
        }
    }

    // on supprime l'objet de la liste d'objets
    objects.erase(std::remove(
        objects.begin(), objects.end(), object
    ), objects.end());
}

std::vector<sf::Vector2f>& Level::getZone() {
    return zone;
}

const std::vector<sf::Vector2f>& Level::getZone() const {
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
