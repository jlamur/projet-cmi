#include "resource_manager.hpp"
#include "whereami.h"
#include <cstring>

// définition du séparateur de fichiers en fonction
// du type de système
#ifdef _WIN32
const std::string SEP = "\\";
#else
const std::string SEP = "/";
#endif

ResourceManager::ResourceManager() : music_volume(5) {
    // on récupère le chemin actuel de l'exécutable pour pouvoir accéder
    // au dossier des ressources qui est situé dans le même dossier
    int length = wai_getExecutablePath(NULL, 0, NULL), dirname_length;
    std::unique_ptr<char[]> buffer = std::unique_ptr<char[]>(new char[length + 1]);
    wai_getExecutablePath(buffer.get(), length, &dirname_length);

    if (length == 0) {
        throw std::runtime_error("Impossible de déterminer le chemin actuel");
    }

    buffer.get()[length] = '\0';
    std::string base_dir = std::string(buffer.get()).substr(0, dirname_length);
    resources_dir = base_dir + SEP + "res" + SEP;

    // initialisation de la musique en bouclage et au volume par défaut
    music.setLoop(true);
    music.setVolume(music_volume);
}

ResourceManager::~ResourceManager() {
    textures.clear();
    fonts.clear();
}

sf::Texture& ResourceManager::getTexture(std::string name) {
    // si la texture est déjà chargée, on l'utilise directement
    if (textures.count(name) > 0) {
        return *textures[name];
    }

    auto texture = std::unique_ptr<sf::Texture>(new sf::Texture);

    // tente de charger la texture dans le chemin "res/textures/name.png"
    if (!texture->loadFromFile(resources_dir + SEP + "textures" + SEP + name)) {
        throw std::runtime_error(
            "Impossible de charger l'image \"" + name + "\""
        );
    }

    textures[name] = std::move(texture);
    return *textures[name];
}

sf::Font& ResourceManager::getFont(std::string name) {
    // si la police est déjà chargée, on l'utilise directement
    if (fonts.count(name) > 0) {
        return *fonts[name];
    }

    auto font = std::unique_ptr<sf::Font>(new sf::Font);

    // tente de charger la police depuis le dossier "res/fonts"
    if (!font->loadFromFile(resources_dir + SEP + "fonts" + SEP + name)) {
        throw std::runtime_error(
            "Impossible de charger la police \"" + name + "\""
        );
    }

    fonts[name] = std::move(font);
    return *fonts[name];
}

std::string ResourceManager::getLevelPath(std::string name) {
    return resources_dir + SEP + "levels" + SEP + name;
}

void ResourceManager::playMusic(std::string name) {
    // tente de charger la musique depuis le dossier "res/musics"
    if (!music.openFromFile(resources_dir + SEP + "musics" + SEP + name)) {
        throw std::runtime_error("Impossible de charger la musique : " + name);
    }

    music.play();
}

void ResourceManager::stopMusic() {
    music.stop();
}

float ResourceManager::getMusicVolume() {
    return music_volume;
}

void ResourceManager::setMusicVolume(float set_music_volume) {
    music_volume = set_music_volume;
    music.setVolume(music_volume);
}
