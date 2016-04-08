#include "constants.hpp"
#include "block.hpp"
#include "player.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <queue>
#include <utility>
#include "level.hpp"

/**
 * Dictionnaire associant les types d'objets
 * à des instances qui seront utilisées pour la
 * construction d'autres objets de ces types
 */
std::map<unsigned int, std::function<ObjectPtr(std::ifstream&)>> object_type_map = {
    {Player::TYPE_ID, Player::load},
    {Block::TYPE_ID, Block::load}
};

Level::Level(Manager& manager) : View(manager), total_time(30) {
    camera = manager.getWindow().getDefaultView();
    camera.setCenter(0, 0);
}

Level::~Level() {
    objects.clear();
}

void Level::load(std::ifstream& file) {
    // vide le niveau précédent s'il y a lieu
    if (objects.size() != 0) {
        objects.clear();
    }

    // positionnement de la caméra au centre du niveau
    camera.setCenter(0, 0);

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
    std::string std_name;
    std::getline(file, std_name, '\0');
    name = sf::String(std_name);

    // lecture du temps total du niveau
    file.read(reinterpret_cast<char*>(&total_time), sizeof(total_time));
    total_time = ntohl(total_time);

    // lecture de la zone de jeu
    char control_points;
    file.read(&control_points, 1);
    zone.clear();

    for (int i = 0; i < control_points; i++) {
        float pos_x, pos_y;

        file.read(reinterpret_cast<char*>(&pos_x), sizeof(pos_x));
        file.read(reinterpret_cast<char*>(&pos_y), sizeof(pos_y));

        pos_x *= Constants::GRID;
        pos_y *= Constants::GRID;

        zone.push_back(std::make_pair(pos_x, pos_y));
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

void Level::save() {
    // TODO: faire une fonction d'enregistrement
}

bool Level::frame() {
    const std::vector<sf::Event>& events = manager.getEvents();

    // traitement des événements
    for (unsigned int i = 0; i < events.size(); i++) {
        if (processEvent(events[i])) {
            // /!\ On arrête là si on a demandé l'interruption.
            // Il est important de ne plus appeler aucune autre
            // fonction de la classe pour éviter une erreur
            // de segmentation
            return true;
        }
    }

    return false;
}

bool Level::processEvent(const sf::Event& event) {
    // lorsque la fenêtre est redimensionnée, mise à jour
    // de la taille de la caméra
    if (event.type == sf::Event::Resized) {
        camera.setSize(event.size.width, event.size.height);
    }

    return false;
}

void Level::draw() {
    sf::RenderWindow& window = manager.getWindow();

    // passage sur la vue caméra
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
        display_queue.top()->draw(manager);
        display_queue.pop();
    }

    // passage sur la vue par défaut
    manager.resetDefaultView();
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

std::vector<ObjectPtr>& Level::getObjects() {
    return objects;
}

std::vector<std::pair<float, float>>& Level::getZone() {
    return zone;
}

sf::View Level::getCamera() {
    return camera;
}

sf::Vector2f Level::convertCoords(sf::Vector2i initial) {
    sf::RenderWindow& window = manager.getWindow();
    sf::View old_view = window.getView();

    window.setView(camera);
    sf::Vector2f converted = window.mapPixelToCoords(initial);
    window.setView(old_view);

    return converted;
}

void Level::setCamera(sf::View set_camera) {
    camera = set_camera;
}
