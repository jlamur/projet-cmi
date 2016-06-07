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
    // le dossier des ressources peut se trouver dans le même dossier
    // que l'exécutable (en mode portable), ou bien dans un dossier
    // du système (en mode installé)

    // détection du dossier de stockage des ressources par défaut
    if (fs::exists(fs::current_path() / "res")) {
        resources_path = fs::current_path() / "res";
    } else {
        resources_path = fs::path("/usr/share/skizzle/res");
    }

    // détection du dossier utilisateur
    const char* home = getenv("HOME");

    if (home == NULL) {
        home = getenv("USERPROFILE");
    }

    if (home == NULL) {
        std::cerr << "Impossible de récupérer le chemin ";
        std::cerr << "vers le dossier utilisateur." << std::endl;
    }

    // dossier des ressources du jeu appartenant à l'utilisateur
    // (si ce dossier n'existe pas, on le crée. On s'assure également
    // que les sous-dossiers soient correctement créés)
    user_path = fs::path(home) / ".skizzle";

    if (!fs::exists(user_path) && !fs::create_directory(user_path)) {
        std::cerr << "Impossible de créer le dossier .skizzle ";
        std::cerr << "dans le dossier utilisateur." << std::endl;
    }

    if (!fs::exists(user_path / "levels") && !fs::create_directory(user_path / "levels")) {
        std::cerr << "Impossible de créer le dossier .skizzle/levels ";
        std::cerr << "dans le dossier utilisateur. " << std::endl;
    }

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

std::vector<fs::path> ResourceManager::getLevels(bool only_user) const {
    // les niveaux peuvent se trouver dans le répertoire du jeu
    // (niveaux par défaut) ou dans le répertoire de l'utilisateur
    // (niveaux personnels)
    fs::recursive_directory_iterator end,
        default_levels(resources_path / "levels"),
        user_levels(user_path / "levels");

    std::vector<fs::path> list;

    // récupération de la liste de tous les niveaux se trouvant dans
    // les dossiers utilisateur et par défaut (en prenant soin de ne garder
    // que les vrais fichiers)
    std::function<bool(fs::path)> filter(
        (bool(*)(const fs::path&)) fs::is_regular_file
    );

    if (!only_user) {
        std::copy_if(default_levels, end, std::back_inserter(list), filter);
        std::sort(list.begin(), list.end());
    }

    std::vector<fs::path>::iterator user_boundary = list.end();
    std::copy_if(user_levels, end, std::back_inserter(list), filter);
    std::sort(user_boundary, list.end());

    return list;
}

fs::path ResourceManager::getThemePath() const {
    return resources_path / "gui.theme";
}

fs::path ResourceManager::getImagesPath() const {
    return resources_path / "images";
}

fs::path ResourceManager::getTexturesPath() const {
    return resources_path / "textures";
}

fs::path ResourceManager::getFontsPath() const {
    return resources_path / "fonts";
}

fs::path ResourceManager::getMusicsPath() const {
    return resources_path / "musics";
}

std::shared_ptr<sf::Image> ResourceManager::getImage(fs::path path) {
    std::string path_str = path.string();

    // si l'image a déjà été chargée, on renvoie la version en cache
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

// surcharges prenant en charge des types d'arguments
// plus traditionnels (chaînes de caractères du C/C++)
std::shared_ptr<sf::Image> ResourceManager::getImage(std::string name) {
    return getImage(getImagesPath() / name);
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
    return getTexture(getTexturesPath() / name);
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
    return getFont(getFontsPath() / name);
}

std::shared_ptr<sf::Font> ResourceManager::getFont(const char* name) {
    return getFont(std::string(name));
}

void ResourceManager::playMusic(fs::path path) {
    // si la musique est déjà chargée, on la relance
    // si elle est en pause, sinon on ne fait rien
    if (current_music_path == path) {
        if (!is_playing) {
            is_playing = true;
            current_music.play();
        }

        return;
    }

    // tente de charger la musique depuis le dossier des musiques
    if (!current_music.openFromFile(path.string())) {
        std::cerr << "Impossible de lire la musique :" << std::endl;
        std::cerr << path.string() << std::endl << std::endl;
    }

    current_music_path = path;
    is_playing = true;
    current_music.play();
}

void ResourceManager::playMusic(std::string name) {
    playMusic(getMusicsPath() / name);
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
