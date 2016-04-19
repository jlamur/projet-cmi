#ifndef __SKIZZLE_RESOURCE_MANAGER_HPP__
#define __SKIZZLE_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <fstream>
#include <memory>

/**
 * Gestionnaire de ressources du jeu. Conserve des
 * références vers toutes les ressources pour éviter
 * de les charger deux fois, permet l'accès uniforme
 * aux ressources
 */
class ResourceManager {
private:
    bool preloaded;

    boost::filesystem::path textures_path;
    boost::filesystem::path fonts_path;
    boost::filesystem::path levels_path;
    boost::filesystem::path musics_path;

    std::unordered_map<std::string, std::unique_ptr<sf::Image>> images;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;
    sf::Music music;

    float music_volume;
    bool playing_state;
    std::string current_music;

public:
    ResourceManager();

    /**
     * Précharge toutes les ressources préchargeables
     */
    void preload();

    /**
     * Récupère une image préchargée
     */
    sf::Image& getImage(std::string name);

    /**
     * Récupère une texture préchargée
     */
    sf::Texture& getTexture(std::string name);

    /**
     * Récupère une police préchargée
     */
    sf::Font& getFont(std::string name);

    /**
     * Récupère le chemin vers le fichier du niveau portant le
     * nom passé en argument
     */
    std::string getLevelPath(std::string name);

    /**
     * Récupère la liste de tous les niveaux
     */
    std::vector<std::string> getLevelList();

    /**
     * Démarre la musique de fond donnée
     */
    void playMusic(std::string name);

    /**
     * Arrête la musique de fond
     */
    void stopMusic();

    /**
     * Récupère le volume de la musique de fond
     */
    float getMusicVolume();

    /**
     * Modifie le volume de la musique de fond
     */
    void setMusicVolume(float set_music_volume);
};

#endif
