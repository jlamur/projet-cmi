#ifndef __PTF_OBJECT_VIEW_HPP__
#define __PTF_OBJECT_VIEW_HPP__

#include <fstream>
#include <vector>
#include "view.hpp"
#include "object.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"

/**
 * La classe Level est une abstraction des
 * classes affichant une collection d'objets, comme
 * les classe de l'éditeur et du jeu
 */
class Level : public View {
private:
    sf::String name;
    int total_time;
    sf::Sprite background;

    std::vector<ObjectPtr> objects;
    std::vector<std::pair<float, float>> zone;

protected:
    /**
     * Dessine tous les objets et le fond à l'écran
     */
    virtual void draw();

public:
    Level(Manager& manager);
    virtual ~Level();

    /**
     * Charge un niveau de jeu depuis le fichier donné
     */
    virtual void load(std::ifstream& file);

    /**
     * Sauvegarde la configuration actuelle comme un niveau
     */
    virtual void save();

    /**
     * Récupère le nom du niveau
     */
    sf::String getName() const;

    /**
     * Modifie le nom du niveau
     */
    void setName(sf::String set_name);

    /**
     * Récupère le temps total alloué pour terminer le niveau
     */
    int getTotalTime() const;

    /**
     * Modifie le temps total du niveau
     */
    void setTotalTime(int set_total_time);

    /**
     * Récupère le fond du niveau
     */
    sf::Sprite getBackground() const;

    /**
     * Modifie le fond du niveau
     */
    void setBackground(sf::Sprite set_background);

    /**
     * Récupère la liste des objets
     */
    std::vector<ObjectPtr>& getObjects();

    /**
     * Récupère la zone du niveau
     */
    std::vector<std::pair<float, float>>& getZone();
};

#endif
