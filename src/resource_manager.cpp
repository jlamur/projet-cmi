#include <iostream>
#include <utility>
#include "resource_manager.hpp"

namespace fs = boost::filesystem;

ResourceManager& ResourceManager::get() {
    static ResourceManager manager;
    return manager;
}

ResourceManager::ResourceManager() : is_playing(false), music_volume(20) {
    // mise en mémoire des chemins vers les dossiers de ressources
    fs::path res_path = fs::current_path() / "res";

    textures_path = res_path / "textures";
    fonts_path = res_path / "fonts";
    levels_path = res_path / "levels";
    musics_path = res_path / "musics";

    // initialisation de la musique en bouclage et au volume par défaut
    current_music.setLoop(true);
    current_music.setVolume(music_volume);
}

std::vector<fs::path> ResourceManager::getFiles(fs::path path) const {
    fs::recursive_directory_iterator dir(path), end;
    std::vector<fs::path> result;

    // on boucle sur tous les fichiers du dossier
    // et de ses sous-dossiers et on les garde en mémoire
    while (dir != end) {
        if (fs::is_regular_file(dir->path())) {
            result.push_back(dir->path());
        }

        ++dir;
    }

    return result;
}

const fs::path& ResourceManager::getTexturesPath() const {
    return textures_path;
}

const fs::path& ResourceManager::getFontsPath() const {
    return fonts_path;
}

const fs::path& ResourceManager::getLevelsPath() const {
    return levels_path;
}

const fs::path& ResourceManager::getMusicsPath() const {
    return musics_path;
}

std::shared_ptr<sf::Image> ResourceManager::getImage(fs::path path) {
    std::string path_str = path.string();

    // si l'image a déjà été chargée, on retourne la
    // version en cache mémoire
    if (images_cache.count(path_str) > 0) {
        return images_cache[path_str];
    }

    // on tente de charger l'image depuis son emplacement
    auto image = std::shared_ptr<sf::Image>(new sf::Image);

    if (!image->loadFromFile(path_str)) {
        std::cerr << "Impossible de charger l'image :" << std::endl;
        std::cerr << path_str << std::endl << std::endl;
    }

    // on met en cache l'image pour les requêtes suivantes
    // puis on la renvoie
    images_cache[path_str] = std::move(image);
    return images_cache[path_str];
}

std::shared_ptr<sf::Image> ResourceManager::getImage(std::string name) {
    return getImage(textures_path / name);
}

std::shared_ptr<sf::Image> ResourceManager::getImage(const char* name) {
    return getImage(std::string(name));
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(fs::path path) {
    std::string path_str = path.string();

    // si la texture est déjà dans le GPU, on renvoie son pointeur
    if (textures_cache.count(path_str) > 0) {
        return textures_cache[path_str];
    }

    // on récupère l'image depuis le disque
    std::shared_ptr<sf::Image> image = getImage(path);

    // on transfère l'image vers le GPU
    auto texture = std::shared_ptr<sf::Texture>(new sf::Texture);
    texture->setSmooth(true);

    if (!texture->loadFromImage(*image)) {
        std::cerr << "Impossible de créer la texture :" << std::endl;
        std::cerr << path_str << std::endl << std::endl;
    }

    // on met en cache la texture pour les requêtes suivantes
    // puis on la renvoie
    textures_cache[path_str] = std::move(texture);
    return textures_cache[path_str];
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(std::string name) {
    return getTexture(textures_path / name);
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(const char* name) {
    return getTexture(std::string(name));
}

std::shared_ptr<sf::Font> ResourceManager::getFont(fs::path path) {
    // on ne maintient pas de cache pour les polices, car ceci
    // est géré par la librairie du GUI (SFGUI). On tente de
    // charger la police depuis le disque
    auto font = std::shared_ptr<sf::Font>(new sf::Font);

    if (!font->loadFromFile(path.string())) {
        std::cerr << "Impossible de charger la police :" << std::endl;
        std::cerr << path.string() << std::endl << std::endl;
    }

    return font;
}

std::shared_ptr<sf::Font> ResourceManager::getFont(std::string name) {
    return getFont(fonts_path / name);
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const char* name) {
    return getFont(std::string(name));
}

void ResourceManager::playMusic(fs::path path) {
    // si la musique est déjà chargée, on la relance si elle
    // est en pause, sinon on ne fait rien
    if (current_music_path == path) {
        if (!is_playing) {
            is_playing = true;
            current_music.play();
        }

        return;
    }

    // tente de charger la musique depuis le dossier "res/musics"
    if (!current_music.openFromFile(path.string())) {
        std::cerr << "Impossible de lire la musique :" << std::endl;
        std::cerr << path.string() << std::endl << std::endl;
    }

    current_music_path = path;
    is_playing = true;
    current_music.play();
}

void ResourceManager::playMusic(std::string name) {
    playMusic(musics_path / name);
}

void ResourceManager::playMusic(const char* name) {
    playMusic(std::string(name));
}

void ResourceManager::stopMusic() {
    is_playing = false;
    current_music.stop();
}

float ResourceManager::getMusicVolume() const {
    return music_volume;
}

void ResourceManager::setMusicVolume(float set_music_volume) {
    music_volume = set_music_volume;
    current_music.setVolume(music_volume);
}
