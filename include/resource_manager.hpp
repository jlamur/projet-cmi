#ifndef __SKIZZLE_RESOURCE_MANAGER_HPP__
#define __SKIZZLE_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFGUI/Image.hpp>
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
    boost::filesystem::path images_path;
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
    bool is_muted;

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
     * Récupère le chemin vers le dossier des images
     */
    const boost::filesystem::path& getImagesPath() const;

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
    std::shared_ptr<sf::Image> getImage(boost::filesystem::path path);
    std::shared_ptr<sf::Image> getImage(std::string name);
    std::shared_ptr<sf::Image> getImage(const char* name);

    /**
     * Charge l'image dont le chemin est donné en paramètre
     * et la charge vers le GPU en tant que texture
     */
    std::shared_ptr<sf::Texture> getTexture(boost::filesystem::path path);
    std::shared_ptr<sf::Texture> getTexture(std::string name);
    std::shared_ptr<sf::Texture> getTexture(const char* name);

    /**
     * Récupère une police préchargée
     */
    std::shared_ptr<sf::Font> getFont(boost::filesystem::path path);
    std::shared_ptr<sf::Font> getFont(std::string name);
    std::shared_ptr<sf::Font> getFont(const char* name);

    /**
     * Joue la musique de fond donnée en paramètre
     */
    void playMusic(boost::filesystem::path path);
    void playMusic(std::string name);
    void playMusic(const char* name);

    /**
     * Arrête la musique de fond
     */
    void stopMusic();

    /**
     * Récupère si le volume est muet ou non
     */
    bool isMuted() const;

    /**
     * Modifie le drapeau pour le muet
     */
    void setMuted(bool set_muted);
};

#endif
