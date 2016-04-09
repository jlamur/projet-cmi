#ifndef __PTF_OBJECT_VIEW_HPP__
#define __PTF_OBJECT_VIEW_HPP__

#include <fstream>
#include <vector>
#include "view.hpp"
#include "object.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"

// liste des directions de la gravité
enum class GravityDirection {NORTH, EAST, SOUTH, WEST};

/**
 * La classe Level est une abstraction des
 * classes affichant une collection d'objets, comme
 * les classe de l'éditeur et du jeu
 */
class Level : public View {
private:
    sf::View camera;
    sf::String name;
    int total_time;
    sf::Sprite background;
    std::string music_name;

    sf::Vector2f gravity;
    std::vector<ObjectPtr> objects;
    std::vector<std::pair<float, float>> zone;

protected:

    /**
     * Dessine tous les objets et le fond à l'écran
     */
    virtual void draw();

    /**
     * Traite un événement
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Convertit les coordonnées à l'écran en coordonnées du monde
     * d'après la vue caméra
     */
    sf::Vector2f pixelToCoords(sf::Vector2i pixel);

    /**
     * Convertit les coordonnées du monde en coordonnées à l'écran
     * d'après la vue caméra
     */
    sf::Vector2i coordsToPixel(sf::Vector2f coords);

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
     * Appelé par le manager lorsque la vue commence à
     * être utilisée
     */
    virtual void begin();

    /**
     * Demande le passage à la frame suivante sur cette vue
     */
    virtual void frame(const std::vector<sf::Event>& events);

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
     * Récupère le vecteur gravité
     */
    sf::Vector2f getGravity() const;

    /**
     * Modifie la direction de la gravité
     */
    void setGravity(GravityDirection direction);

    /**
     * Récupère la liste des objets
     */
    std::vector<ObjectPtr>& getObjects();

    /**
     * Récupère la liste des objets (version constante)
     */
    const std::vector<ObjectPtr>& getObjects() const;

    /**
     * Récupère la zone du niveau
     */
    std::vector<std::pair<float, float>>& getZone();

    /**
     * Récupère la zone du niveau (version constante)
     */
    const std::vector<std::pair<float, float>>& getZone() const;

    /**
     * Récupère la caméra actuelle
     */
    sf::View getCamera() const;

    /**
     * Modifie la caméra
     */
    void setCamera(sf::View set_camera);
};

#endif
