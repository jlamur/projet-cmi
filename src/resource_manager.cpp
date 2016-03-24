#include "resource_manager.hpp"
#include "whereami.h"
#include <iostream>

#ifdef _WIN32
    #define FILE_SEP '\\'
#else
    #define FILE_SEP '/'
#endif

ResourceManager::~ResourceManager() {
    textures.clear();
}

/**
 * Récupère le chemin actuel de l'exécutable sous la forme
 * d'une chaîne de caractères grâce à la librairie whereami
 */
std::string getCurrentDirectory() {
    int length = wai_getExecutablePath(NULL, 0, NULL), dirname_length;
    char* buffer = new char[length + 1];
    wai_getExecutablePath(buffer, length, &dirname_length);

    if (dirname_length == 0) {
        throw std::runtime_error("Impossible de déterminer le chemin actuel");
    }

    buffer[length] = '\0';
    return std::string(buffer).substr(0, dirname_length);
}

sf::Texture& ResourceManager::getTexture(std::string name) {
    // si la texture est déjà chargée, on l'utilise directement
    if (textures.count(name) > 0) {
        return textures[name];
    }

    sf::Texture texture;
    std::string path = getCurrentDirectory() + FILE_SEP + "res" + FILE_SEP + name;

    // tente de charger la texture dans le chemin "CWD/res/name"
    if (!texture.loadFromFile(path)) {
        throw std::runtime_error("Impossible de charger l'image: " + name);
    }

    textures[name] = texture;
    return textures[name];
}
