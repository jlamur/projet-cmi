#ifndef __PTF_LEVEL_HPP__
#define __PTF_LEVEL_HPP__

#include <fstream>
#include <vector>
#include "state.hpp"
#include "object.hpp"
#include "manager.hpp"
#include "resource_manager.hpp"

// liste des directions de la gravité
enum class GravityDirection {NORTH, EAST, SOUTH, WEST};

/**
 * La classe Level est une abstraction des
 * classes affichant une collection d'objets, comme
 * les classes de l'éditeur et du jeu
 */
class Level : public State {
private:
    sf::View camera;
    float camera_angle;
    GravityDirection gravity_direction;

    sf::String name;
    int total_time;

    sf::Sprite background_sprite;
    std::string background;
    std::string music;

    std::vector<ObjectPtr> objects;
    std::vector<std::pair<float, float>> zone;

protected:

    /**
     * Dessine tous les objets et le fond à l'écran
     */
    virtual void draw();

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
     * Charge un niveau de jeu avec le nom donné
     */
    virtual void load(std::string filename);

    /**
     * Sauvegarde la configuration actuelle dans le niveau donné
     */
    virtual void save(std::string filename);

    /**
     * Appelé par le manager lorsque l'état est utilisé
     */
    virtual void begin();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

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
     * Récupère la musique du niveau
     */
    std::string getMusic() const;

    /**
     * Modifie la musique du niveau
     */
    void setMusic(std::string set_music);

    /**
     * Récupère le fond du niveau
     */
    std::string getBackground() const;

    /**
     * Modifie le fond du niveau
     */
    void setBackground(std::string set_background);

    /**
     * Calcule le vecteur gravité en fonction de la direction de la gravité
     */
    sf::Vector2f getGravity() const;

    /**
     * Calcule la direction gauche en fonction de la direction de la gravité
     */
    sf::Vector2f getLeftDirection() const;

    /**
     * Calcule la direction droite en fonction de la direction de la gravité
     */
    sf::Vector2f getRightDirection() const;

    /**
     * Récupère la direction de la gravité
     */
    GravityDirection getGravityDirection();

    /**
     * Modifie la direction de la gravité
     */
    void setGravityDirection(GravityDirection set_gravity_direction);

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
