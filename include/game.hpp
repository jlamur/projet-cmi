#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include "level.hpp"
#include "editor.hpp"

/**
 * La classe Game gère l'affichage et les objets
 * d'une partie de jeu
 */
class Game : public Level {
public:
    /**
     * Les différents modes de jeu
     */
    enum class Mode {NORMAL, PAUSED, WON, LOST};

private:
    WidgetTimer widget_timer;
    sf::Time next_frame_time;
    std::vector<Object::Ptr> pending_kill;

    bool test_mode;
    float time_left;

    std::shared_ptr<Editor> return_state;
    Mode mode;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update();

    /**
     * Vérifie si l'objet donné se trouve dans
     * la zone de jeu ou non
     */
    bool isInZone(Object::Ptr object);

protected:
    /**
     * Dessine tous les objets, le fond et
     * l'interface de jeu
     */
    virtual void draw();

    /**
     * S'assure que la caméra est centrée sur les joueurs
     */
    void ensureCentered();

public:
    Game(Manager& manager);
    virtual ~Game();

    /**
     * Appelé par le manager lorsque l'état est utilisé
     */
    virtual void begin();

    /**
     * Traite l'événement donné
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * Demande le dessin d'une frame
     */
    virtual void frame();

    /**
     * Récupère si on est en mode test ou pas
     */
    bool getTestMode();

    /**
     * Mise en mode test : l'appui sur espace renvoie
     * vers l'éditeur donné
     */
    void setTestMode(std::shared_ptr<Editor> set_return_state);

    /**
     * Récupère le mode actuel de jeu
     */
    Mode getMode();

    /**
     * Modifie le mode actuel de jeu
     */
    void setMode(Mode set_mode);
};

#endif
