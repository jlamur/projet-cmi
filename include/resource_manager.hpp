#ifndef __SKIZZLE_RESOURCE_MANAGER_HPP__
#define __SKIZZLE_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <string>
#include <memory>

/**
 * Gestionnaire de ressources du jeu. La classe agit comme une interface
 * avec le dossier res/. Elle permet de précharger les ressources gourmandes
 * telles que les textures ou les polices
 */
class ResourceManager {
private:
    /**
     * Chemins vers les différents dossiers de ressources
     */
    boost::filesystem::path textures_path;
    boost::filesystem::path fonts_path;
    boost::filesystem::path levels_path;
    boost::filesystem::path musics_path;

    std::unordered_map<
        std::string,
        std::shared_ptr<sf::Image>
    > images_cache;

    std::unordered_map<
        std::string,
        std::shared_ptr<sf::Texture>
    > textures_cache;

    boost::filesystem::path current_music_path;
    bool is_playing;
    sf::Music current_music;
    float music_volume;

    /**
     * Construit le gestionnaire de ressources. Comme on ne
     * veut qu'une seule instance du gestionnaire, c'est privé
     */
    ResourceManager();

public:
    /**
     * Renvoie l'unique instance du singleton gestionnaire de ressources
     */
    static ResourceManager& get();

    /**
     * Récupère la liste des fichiers dans le dossier donné
     */
    std::vector<boost::filesystem::path> getFiles(boost::filesystem::path path) const;

    /**
     * Récupère le chemin vers le dossier des textures
     */
    const boost::filesystem::path& getTexturesPath() const;

    /**
     * Récupère le chemin vers le dossier des polices
     */
    const boost::filesystem::path& getFontsPath() const;

    /**
     * Récupère le chemin vers le dossier des niveaux
     */
    const boost::filesystem::path& getLevelsPath() const;

    /**
     * Récupère le chemin vers le dossier des musiques
     */
    const boost::filesystem::path& getMusicsPath() const;

    /**
     * Charge l'image dont le chemin est donné en paramètre
     */
    std::shared_ptr<sf::Image> getImage(std::string name);

    /**
     * Charge l'image dont le chemin est donné en paramètre
     * et la charge vers le GPU en tant que texture
     */
    std::shared_ptr<sf::Texture> getTexture(std::string name);

    /**
     * Récupère une police préchargée
     */
    std::shared_ptr<sf::Font> getFont(std::string name);

    /**
     * Joue la musique de fond donnée en paramètre
     */
    void playMusic(std::string name);

    /**
     * Arrête la musique de fond
     */
    void stopMusic();

    /**
     * Récupère le volume de la musique de fond
     */
    float getMusicVolume() const;

    /**
     * Modifie le volume de la musique de fond
     */
    void setMusicVolume(float set_music_volume);
};

#endif
