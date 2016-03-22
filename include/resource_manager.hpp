#ifndef __PTF_RESOURCE_MANAGER_HPP__
#define __PTF_RESOURCE_MANAGER_HPP__

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
private:
    std::map<std::string, sf::Texture> textures;

public:
    ~ResourceManager();

    /**
     * Récupère une image. Réutilise l'image déjà chargée
     * si elle l'a déjà été, sinon, tente de la charger
     * depuis son emplacement
     */
    sf::Texture& getTexture(std::string name);
};

#endif
