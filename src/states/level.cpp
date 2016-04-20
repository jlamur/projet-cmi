#include <boost/filesystem.hpp>
#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include "manager.hpp"
#include "resource_manager.hpp"
#include "utility.hpp"
#include "objects/player.hpp"
#include "objects/block.hpp"
#include "objects/gravity_block.hpp"
#include "objects/switch_block.hpp"
#include "objects/finish_block.hpp"
#include "objects/kill_block.hpp"
#include "states/level.hpp"

/**
 * Définition des variables et fonctions globales internes
 * (accessibles uniquement dans ce fichier)
 */
namespace {
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
}

Level::Level(Manager& manager) : State(manager) {
    gravity_direction = GravityDirection::SOUTH;

    // métadonnées par défaut
    setName(sf::String("Nouveau niveau"));
    setPath((ResourceManager::get().getLevelsPath() / "new_level.dat").string());
    setTotalTime(30);

    // zone de jeu par défaut
    int size = 8;

    zone.push_back(sf::Vector2f(-size * Manager::GRID, -size * Manager::GRID));
    zone.push_back(sf::Vector2f(size * Manager::GRID, -size * Manager::GRID));
    zone.push_back(sf::Vector2f(size * Manager::GRID, size * Manager::GRID));
    zone.push_back(sf::Vector2f(-size * Manager::GRID, size * Manager::GRID));

    // ressources par défaut
    setMusic("");
    setBackground("");
}

Level::~Level() {}

void Level::enable() {
    // positionnement par défaut de la caméra
    sf::Vector2u window_size = getManager().getWindow().getSize();

    camera.setSize(window_size.x, window_size.y);
    camera.setCenter(0, 0);
    camera_angle = 180.f;
}

void Level::load() {
    // ouverture du fichier
    std::ifstream file;
    file.open(path, std::ios::binary | std::ios::in);

    // on vérifie que le fichier ait correctement été ouvert en lecture
    if (file.fail()) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + path + "\" " +
            "(" + std::string(strerror(errno)) + ")"
        );
    }

    // lecture de la signature du fichier ("BAR")
    char read_signature[3];
    file.read(read_signature, 3);

    if (strncmp(read_signature, "BAR", 3) != 0) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + path + "\" " +
            "(en-tête invalide)"
        );
    }

    // lecture de la version du fichier
    char read_file_version;
    file.read(&read_file_version, 1);

    if (read_file_version != VERSION_NUMBER) {
        throw std::runtime_error(
            "Impossible de charger le niveau \"" + path + "\" " +
            "(version non prise en charge)"
        );
    }

    // lecture du nom du niveau
    std::string read_name;
    std::getline(file, read_name, '\0');
    setName(read_name);

    // lecture du temps total du niveau
    int read_total_time;
    file.read(reinterpret_cast<char*>(&read_total_time), 4);
    setTotalTime(ntohl(read_total_time));

    // lecture de la zone de jeu
    char read_control_points;
    file.read(&read_control_points, 1);
    zone.clear();

    for (int i = 0; i < read_control_points; i++) {
        float read_pos_x, read_pos_y;

        file.read(reinterpret_cast<char*>(&read_pos_x), 4);
        file.read(reinterpret_cast<char*>(&read_pos_y), 4);

        read_pos_x *= Manager::GRID;
        read_pos_y *= Manager::GRID;

        zone.push_back(sf::Vector2f(read_pos_x, read_pos_y));
    }

    // lecture des chemins de la musique et du fond
    std::string read_music;
    std::string read_background;

    std::getline(file, read_music, '\0');
    std::getline(file, read_background, '\0');

    setMusic(read_music);
    setBackground(read_background);

    // lecture des objets si une callback a été fournie
    int read_object_count;
    file.read(reinterpret_cast<char*>(&read_object_count), 4);
    read_object_count = ntohl(read_object_count);

    for (int i = 0; i < read_object_count; i++) {
        char read_object_type;
        file.read(&read_object_type, 1);

        // vérifie que le type est pris en charge
        // pour éviter une erreur de segmentation
        if (object_type_map.count(read_object_type) == 0) {
            throw std::runtime_error(
                "Impossible de charger le niveau \"" + path + "\" " +
                "(type d'objet " + std::to_string(read_object_type) + " inconnu)"
            );
        }

        // lecture de l'objet
        addObject(object_type_map[read_object_type](file));
    }
}

void Level::save() {
    std::ofstream file;
    file.open(path, std::ios::binary | std::ios::out);

    // on vérifie que le fichier ait correctement été ouvert en lecture
    if (file.fail()) {
        throw std::runtime_error(
            "Impossible d'enregistrer le niveau \"" + path + "\" " +
            "(" + std::string(strerror(errno)) + ")"
        );
    }

    // écriture de la signture du fichier ("BAR")
    char write_signature[3] = {'B', 'A', 'R'};
    file.write(write_signature, 3);

    // écriture de la version du fichier
    char write_file_version = VERSION_NUMBER;
    file.write(&write_file_version, 1);

    // écriture du nom du niveau
    char *write_name = (char*) name.toAnsiString().data();
    file.write(write_name, name.getSize() + 1);

    // écriture du temps total du niveau
    int write_total_time = htonl(total_time);
    file.write(reinterpret_cast<char*>(&write_total_time), 4);

    // écriture de la zone de jeu
    char write_control_points = (char) zone.size();
    file.write(&write_control_points, 1);

    for (int i = 0; i < write_control_points; i++) {
        float write_pos_x = zone[i].x / Manager::GRID;
        float write_pos_y = zone[i].y / Manager::GRID;

        file.write(reinterpret_cast<char*>(&write_pos_x), 4);
        file.write(reinterpret_cast<char*>(&write_pos_y), 4);
    }

    // écriture des noms de la musique et du fond
    char *write_music = (char*) music.data();
    file.write(write_music, music.size() + 1);

    char *write_background = (char*) background.data();
    file.write(write_background, background.size() + 1);

    // écriture des objets
    int write_object_count = htonl(objects.size());
    file.write(reinterpret_cast<char*>(&write_object_count), 4);

    for (unsigned int i = 0; i < objects.size(); i++) {
        char write_object_type = objects[i]->getTypeId();
        file.write(&write_object_type, 1);

        // écriture de l'objet
        objects[i]->save(file);
    }
}

std::string Level::getPath() {
    return path;
}

void Level::setPath(std::string set_path ){
    path = set_path;
}

void Level::processEvent(const sf::Event& event) {
    // lorsque la fenêtre est redimensionnée, mise à jour
    // de la taille de la caméra
    if (event.type == sf::Event::Resized) {
        camera.setSize(event.size.width, event.size.height);
    }
}

void Level::draw() {
    sf::RenderWindow& window = getManager().getWindow();
    sf::Vector2u window_size = window.getSize();

    // animation de la rotation de la caméra
    camera_angle = Utility::animateValue(
        camera_angle, 5,
        std::fmod((float) gravity_direction * 90, 360)
    );

    camera.setRotation(camera_angle + 180);
    window.setView(camera);

    // efface la scène précédente
    window.clear(sf::Color(66, 165, 245));

    // on englobe la caméra dans un rectangle circonscrit, dans
    // le cas où elle serait en rotation
    sf::Vector2f camera_corners[] = {
        pixelToCoords(sf::Vector2i(0, 0)),
        pixelToCoords(sf::Vector2i(window_size.x, 0)),
        pixelToCoords(sf::Vector2i(window_size.x, window_size.y)),
        pixelToCoords(sf::Vector2i(0, window_size.y))
    };

    sf::Vector2f camera_topleft = camera_corners[0];
    sf::Vector2f camera_botright = camera_corners[0];

    for (unsigned int i = 0; i < 4; i++) {
        camera_topleft.x = std::min(camera_topleft.x, camera_corners[i].x);
        camera_topleft.y = std::min(camera_topleft.y, camera_corners[i].y);
        camera_botright.x = std::max(camera_botright.x, camera_corners[i].x);
        camera_botright.y = std::max(camera_botright.y, camera_corners[i].y);
    }

    // on dessine le fond s'il y en a un
    if (background != "") {
        auto bg_texture = ResourceManager::get().getTexture("levels/" + background);
        sf::Vector2f bg_size = (sf::Vector2f) bg_texture->getSize();

        background_sprite.setTexture(*bg_texture);

        // on regarde la position du coin où il faut commencer à
        // dessiner le fond, et la zone sur laquelle il faut le dessiner
        int x_left = std::floor(camera_topleft.x / bg_size.x);
        int y_top = std::floor(camera_topleft.y / bg_size.y);
        int x_right = std::ceil(camera_botright.x / bg_size.x);
        int y_bottom = std::ceil(camera_botright.y / bg_size.y);

        // et on dessine aux positions calculées
        for (int x = x_left; x < x_right; x++) {
            for (int y = y_top; y < y_bottom; y++) {
                background_sprite.setPosition(sf::Vector2f(
                    x * (bg_size.x), y * (bg_size.y)
                ));

                window.draw(background_sprite);
            }
        }
    }

    // tri des objets par ordre d'affichage
    std::sort(
        objects.begin(), objects.end(),
        [](const Object::Ptr &a, const Object::Ptr &b) {
            sf::Vector2f a_pos = a->getPosition();
            sf::Vector2f b_pos = b->getPosition();

            return a_pos.x - a_pos.y + a->getLayer()
                 < b_pos.x - b_pos.y + b->getLayer();
        }
    );

    // dessin des objets de la file d'affichage couche par couche
    for (auto it = objects.begin(); it != objects.end(); it++) {
        sf::Vector2i screen_pos = coordsToPixel((*it)->getPosition());
        sf::FloatRect aabb = (*it)->getAABB();

        // si l'objet est visible à l'écran, on le dessine
        if (screen_pos.x >= -aabb.width && screen_pos.y >= -aabb.height &&
            screen_pos.x <= window_size.x + aabb.width &&
            screen_pos.y <= window_size.y + aabb.height) {
            (*it)->draw(*this);
        }
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
    sf::RenderWindow& window = getManager().getWindow();
    sf::View old_view = window.getView();

    window.setView(camera);
    sf::Vector2f coords = window.mapPixelToCoords(pixel);
    window.setView(old_view);

    return coords;
}

sf::Vector2i Level::coordsToPixel(sf::Vector2f coords) {
    sf::RenderWindow& window = getManager().getWindow();
    sf::View old_view = window.getView();

    window.setView(camera);
    sf::Vector2i pixel = window.mapCoordsToPixel(coords);
    window.setView(old_view);

    return pixel;
}

void Level::setCamera(sf::View set_camera) {
    camera = set_camera;
}
