#include "resource_manager.hpp"
#include <iostream>
#include <algorithm>

using dir_iter = boost::filesystem::directory_iterator;
using fs_path = boost::filesystem::path;

ResourceManager::ResourceManager() : preloaded(false),
    music_volume(20), playing_state(false), current_music("") {
    // initialisation de la musique en bouclage et au volume par défaut
    music.setLoop(true);
    music.setVolume(music_volume);
}

void ResourceManager::preload() {
    if (preloaded) {
        return;
    }

    fs_path current = boost::filesystem::current_path();
    dir_iter end;

    // on garde une référence aux chemins des différentes ressources
    textures_path = current / "res/textures";
    fonts_path = current / "res/fonts";
    levels_path = current / "res/levels";
    musics_path = current / "res/musics";

    // préchargement de toutes les textures
    for (dir_iter it(textures_path); it != end; ++it) {
        if (boost::filesystem::is_regular_file(it->path())) {
            std::string full_path = boost::filesystem::canonical(it->path()).string();
            std::string name = it->path().filename().string();

            auto texture = std::unique_ptr<sf::Texture>(new sf::Texture);
            texture->setSmooth(true);
            std::cout << "Chargement de la texture " << name << "... ";

            if (!texture->loadFromFile(full_path)) {
                std::cerr << "ERREUR!" << std::endl;
            } else {
                std::cout << "OK!" << std::endl;
            }

            textures[name] = std::move(texture);
        }
    }

    // préchargement de toutes les polices
    for (dir_iter it(fonts_path); it != end; ++it) {
        if (boost::filesystem::is_regular_file(it->path())) {
            std::string full_path = boost::filesystem::canonical(it->path()).string();
            std::string name = it->path().filename().string();

            auto font = std::unique_ptr<sf::Font>(new sf::Font);
            std::cout << "Chargement de la police " << name << "... ";

            if (!font->loadFromFile(full_path)) {
                std::cerr << "ERREUR!" << std::endl;
            } else {
                std::cout << "OK!" << std::endl;
            }

            fonts[name] = std::move(font);
        }
    }

    preloaded = true;
}

sf::Texture& ResourceManager::getTexture(std::string name) {
    if (textures.count(name) == 0) {
        throw std::runtime_error(
            "Impossible de charger la texture inexistante : " + name
        );
    }

    return *textures[name];
}

sf::Font& ResourceManager::getFont(std::string name) {
    if (fonts.count(name) == 0) {
        throw std::runtime_error(
            "Impossible de charger la police inexistante : " + name
        );
    }

    return *fonts[name];
}

std::string ResourceManager::getLevelPath(std::string name) {
    return (levels_path / name).string();
}

std::vector<std::string> ResourceManager::getLevelList() {
    boost::filesystem::directory_iterator iter(levels_path);
    std::vector<boost::filesystem::path> list;
    std::vector<std::string> path_list;

    // récupération de la liste de tous les niveaux
    std::copy(
        iter, boost::filesystem::directory_iterator(),
        std::back_inserter(list)
    );

    // tri par ordre alphabétique
    std::sort(list.begin(), list.end());

    // conversion en chemins absolus
    for (auto it = list.begin(); it != list.end(); it++) {
        path_list.push_back((*it).string());
    }

    return path_list;
}

void ResourceManager::playMusic(std::string name) {
    // si la musique est déjà chargée, on la relance si elle
    // est en pause, sinon on ne fait rien
    if (current_music == name) {
        if (!playing_state) {
            playing_state = true;
            music.play();
        }

        return;
    }

    // tente de charger la musique depuis le dossier "res/musics"
    std::string full_path = boost::filesystem::canonical(musics_path / name).string();
    std::cout << "Lecture de la musique " << name << "... ";

    if (!music.openFromFile(full_path)) {
        std::cerr << "ERREUR!" << std::endl;
    } else {
        std::cout << "OK!" << std::endl;
    }

    current_music = name;
    playing_state = true;
    music.play();
}

void ResourceManager::stopMusic() {
    // on n'arrête la musique que si elle ne l'est pas déjà
    if (playing_state) {
        playing_state = false;
        music.stop();
    }
}

float ResourceManager::getMusicVolume() {
    return music_volume;
}

void ResourceManager::setMusicVolume(float set_music_volume) {
    music_volume = set_music_volume;
    music.setVolume(music_volume);
}
