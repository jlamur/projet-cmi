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
     * Stockage du chemin vers les dossiers contenant
     * des ressources du jeu
     */
    boost::filesystem::path resources_path;
    boost::filesystem::path user_path;

    /**
     * Mise en cache des images utilisées dans le jeu
     */
    std::unordered_map<
        std::string,
        std::shared_ptr<sf::Image>
    > images_cache;

    /**
     * Mise en cache des textures (images chargées dans le GPU)
     * utilisées dans le jeu
     */
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
     * Récupère la liste des niveaux pouvant être joués.
     * Si le paramètre only_home est passé à true, seuls les
     * niveaux créés par l'utilisateur sont renvoyés. Le résultat
     * est trié dans l'ordre alphanumérique des noms de fichiers,
     * en gardant les niveaux par défaut en haut de liste et
     * les niveaux de l'utilisateur à la suite
     */
    std::vector<boost::filesystem::path> getLevels(bool only_home = false) const;

    /**
     * Récupère le chemin vers les différents éléments
     * de ressources
     */
    boost::filesystem::path getThemePath() const;
    boost::filesystem::path getImagesPath() const;
    boost::filesystem::path getTexturesPath() const;
    boost::filesystem::path getFontsPath() const;
    boost::filesystem::path getMusicsPath() const;

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

    void stopMusic();
    bool isMuted() const;
    void setMuted(bool set_muted);
};

#endif
