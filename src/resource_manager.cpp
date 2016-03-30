#include "resource_manager.hpp"
#include "whereami.h"
#include <memory>

ResourceManager::ResourceManager() {
    music.setLoop(true);
}

ResourceManager::~ResourceManager() {
    textures.clear();
}

/**
 * Récupère le chemin actuel de l'exécutable sous la forme
 * d'une chaîne de caractères grâce à la librairie whereami
 */
std::string getCurrentDirectory() {
    int length = wai_getExecutablePath(NULL, 0, NULL), dirname_length;
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(length + 1);
    wai_getExecutablePath(buffer.get(), length, &dirname_length);

    if (dirname_length == 0) {
        throw std::runtime_error("Impossible de déterminer le chemin actuel");
    }

    buffer.get()[length] = '\0';
    return std::string(buffer.get()).substr(0, dirname_length);
}

/**
 * Récupère le chemin absolu vers la ressource dont
 * le nom est passé en argument
 */
inline std::string getResourcePath(std::string name) {
#ifdef _WIN32
    return getCurrentDirectory() + "\\res\\" + name;
#else
    return getCurrentDirectory() + "/res/" + name;
#endif
}

sf::Texture& ResourceManager::getTexture(std::string name) {
    // si la texture est déjà chargée, on l'utilise directement
    if (textures.count(name) > 0) {
        return textures[name];
    }

    sf::Texture texture;

    // tente de charger la texture dans le chemin "CWD/res/name"
    if (!texture.loadFromFile(getResourcePath(name))) {
        throw std::runtime_error("Impossible de charger l'image : " + name);
    }

    textures[name] = texture;
    return textures[name];
}

void ResourceManager::setMusic(std::string name) {
    if (!music.openFromFile(getResourcePath(name))) {
        throw std::runtime_error("Impossible de charger la musique : " + name);
    }
}

void ResourceManager::playMusic() {
    music.play();
}

void ResourceManager::pauseMusic() {
    music.pause();
}

void ResourceManager::stopMusic() {
    music.stop();
}
