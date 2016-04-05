#ifndef __PTF_RESOURCE_MANAGER_HPP__
#define __PTF_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

class ResourceManager {
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
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
     * Change la musique en lecture de fond
     * Doit être utilisé pour la lecture en continu
     */
    void setMusic(std::string name);

    /**
     * Démarre la musique de fond
     */
    void playMusic();

    /**
     * Met en pause la musique de fond
     */
    void pauseMusic();

    /**
     * Arrête la musique de fond
     */
    void stopMusic();
};

#endif
