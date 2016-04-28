#include <iostream>
#include <algorithm>
#include <utility>
#include "resource_manager.hpp"

namespace fs = boost::filesystem;

ResourceManager& ResourceManager::get() {
    static ResourceManager manager;
    return manager;
}

ResourceManager::ResourceManager() : is_playing(false), is_muted(false) {
    // mise en mémoire des chemins vers les dossiers de ressources
    fs::path res_path = fs::current_path() / "res";

    images_path = res_path / "images";
    textures_path = res_path / "textures";
    fonts_path = res_path / "fonts";
    levels_path = res_path / "levels";
    musics_path = res_path / "musics";

    // initialisation de la musique en bouclage et au volume par défaut
    current_music.setLoop(true);
    current_music.setVolume(100);
}

std::vector<fs::path> ResourceManager::getFiles(fs::path path) const {
    fs::recursive_directory_iterator dir(path), end;
    std::vector<fs::path> result;

    // récupération de la liste de tous les fichiers
    // (et uniquement des fichiers, d'où le filtre)
    std::copy_if(dir, end, std::back_inserter(result), [](fs::path path){
        return fs::is_regular_file(path);
    });

    // tri par ordre alphabétique
    std::sort(result.begin(), result.end());

    return result;
}

const fs::path& ResourceManager::getImagesPath() const {
    return images_path;
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

sfg::Image::Ptr ResourceManager::getImage(fs::path path) {
    std::string path_str = path.string();

    // si l'image a déjà été chargée, on retourne la
    // version en cache mémoire
    if (images_cache.count(path_str) > 0) {
        // le cache d'images est constitué de pointeurs "faibles", càd
        // que dès que la ressource n'est plus utilisée, le pointeur
        // s'invalide. Ceci parce que SFGUI maintient son propre cache.
        // on doit donc d'abord vérifier que le pointeur est toujours valide
        if (!images_cache[path_str].expired()) {
            return images_cache[path_str].lock();
        }
    }

    // on tente de charger l'image depuis son emplacement
    sf::Image image;

    if (!image.loadFromFile(path_str)) {
        std::cerr << "Impossible de charger l'image :" << std::endl;
        std::cerr << path_str << std::endl << std::endl;
    }

    // création d'une nouvelle image SFGUI avec les infos chargées
    // et mise en cache du résultat
    sfg::Image::Ptr sfg_image = sfg::Image::Create(image);
    images_cache[path_str] = sfg_image;

    return sfg_image;
}

sfg::Image::Ptr ResourceManager::getImage(std::string name) {
    return getImage(images_path / name);
}

sfg::Image::Ptr ResourceManager::getImage(const char* name) {
    return getImage(std::string(name));
}

std::shared_ptr<sf::Texture> ResourceManager::getTexture(fs::path path) {
    std::string path_str = path.string();

    // si la texture est déjà dans le GPU, on renvoie son pointeur
    if (textures_cache.count(path_str) > 0) {
        return textures_cache[path_str];
    }

    // on tente de charger la texture depuis le disque vers le GPU
    auto texture = std::shared_ptr<sf::Texture>(new sf::Texture);
    texture->setSmooth(true);

    if (!texture->loadFromFile(path_str)) {
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

bool ResourceManager::isMuted() const {
    return is_muted;
}

void ResourceManager::setMuted(bool set_muted) {
    is_muted = set_muted;

    if (is_muted) {
        current_music.setVolume(0);
    } else {
        current_music.setVolume(100);
    }
}
