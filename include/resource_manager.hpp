#ifndef __PTF_RESOURCE_MANAGER_HPP__
#define __PTF_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
    std::string resources_dir;

    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

    float music_volume;
    sf::Music music;

public:
    ResourceManager();
    ~ResourceManager();

    /**
     * Récupère une image. Réutilise l'image déjà chargée
     * si elle l'a déjà été, sinon, tente de la charger
     * depuis son emplacement
     */
    sf::Texture& getTexture(std::string name);

    /**
     * Récupère la police demandée. Réutilise une police
     * déjà chargée si elle a déjà été demandée, sinon, la
     * charge depuis son emplacement
     */
    sf::Font& getFont(std::string name);

    /**
     * Récupère un lecteur de fichier vers le niveau donné
     */
    std::ifstream getLevelReader(std::string name);

    /**
     * Récupère un jacob de fichier vers le niveau donné
     */
    std::ofstream getLevelWriter(std::string name);

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
