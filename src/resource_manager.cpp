#include "resource_manager.hpp"
#include <iostream>

// fonctionnalités cross-platform pour les fichiers
// source: http://stackoverflow.com/a/145309/3452708
#include <stdio.h>
#ifdef _WIN32
    #include <direct.h>
    #define getcwd _getcwd
    #define FILE_SEP '\\'
#else
    #include <unistd.h>
    #define FILE_SEP '/'
#endif

ResourceManager::~ResourceManager() {
    textures.clear();
}

/**
 * Récupère le chemin actuel de l'exécutable
 * sous la forme d'une chaîne de caractères
 */
std::string getCurrentDirectory() {
    char buffer[FILENAME_MAX];

    if (getcwd(buffer, sizeof(buffer)) != NULL) {
        return std::string(buffer);
    }

    throw std::runtime_error("Impossible de déterminer le chemin actuel");
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
