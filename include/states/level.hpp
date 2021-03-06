#ifndef __SKIZZLE_LEVEL_HPP__
#define __SKIZZLE_LEVEL_HPP__

#include <fstream>
#include <vector>
#include "../gui/action_toolbar.hpp"
#include "../utility.hpp"
#include "../objects/object.hpp"
#include "../objects/player.hpp"
#include "state.hpp"

class Manager;

/**
 * La classe Level est une abstraction des
 * classes affichant une collection d'objets, comme
 * les classes de l'éditeur et du jeu
 */
class Level : public State {
private:
    sf::View camera;
    float camera_angle;
    sf::Vector2f goal_center;

    Utility::Direction gravity_direction;

    sf::String name;
    std::string path;

    int total_time;

    sf::Sprite background_sprite;
    std::string background;
    std::string music;

    std::vector<Object::Ptr> objects;
    std::vector<Player::Ptr> players;
    std::vector<sf::Vector2f> zone;

    sfg::Button::Ptr mute_button;
    bool previously_muted_state;

protected:
    ActionToolbar action_toolbar;

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();

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
     * Active l'état
     */
    virtual void enable();

    /**
     * Charge le niveau de jeu donné depuis le fichier actuel
     */
    void load();

    /**
     * Sauvegarde la configuration actuelle dans le fichier actuel
     */
    void save();

    /**
     * Récupère le chemin actuel du fichier du niveau
     */
    std::string getPath();

    /**
     * Modifie le chemin actuel du fichier du niveau
     */
    void setPath(std::string set_path);

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
    virtual void setTotalTime(int set_total_time);

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
    Utility::Direction getGravityDirection();

    /**
     * Modifie la direction de la gravité
     */
    void setGravityDirection(Utility::Direction set_gravity_direction);

    /**
     * Récupère la liste des objets
     */
    std::vector<Object::Ptr>& getObjects();

    /**
     * Récupère la liste des objets (constante)
     */
    const std::vector<Object::Ptr>& getObjects() const;

    /**
     * Récupère la liste des joueurs
     */
    std::vector<Player::Ptr>& getPlayers();

    /**
     * Récupère la liste des joueurs (constante)
     */
    const std::vector<Player::Ptr>& getPlayers() const;

    /**
     * Ajoute un nouvel objet. Retourne l'objet
     * si il a bien été ajouté, ou nullptr si l'ajout
     * a été bloqué
     */
    Object::Ptr addObject(Object::Ptr object);

    /**
     * Supprime l'objet donné
     */
    void removeObject(Object::Ptr object);
    void removeObject(Object* object);

    /**
     * Récupère la zone du niveau
     */
    std::vector<sf::Vector2f>& getZone();

    /**
     * Récupère la zone du niveau (version constante)
     */
    const std::vector<sf::Vector2f>& getZone() const;

    /**
     * Récupère la caméra actuelle
     */
    sf::View getCamera() const;

    /**
     * Modifie la caméra
     */
    void setCamera(sf::View set_camera);

    /**
     * Récupère la cible de l'animation de la caméra
     */
    sf::Vector2f getCenterGoal();

    /**
     * Modifie la cible de l'animation de la caméra
     */
    void setCenterGoal(sf::Vector2f set_center);

    /**
     * Récupère le centre de tous les joueurs ou
     * zéro si aucun joueur
     */
    sf::Vector2f getPlayerCenter();
};

#endif
