#ifndef __PTF_GAME_HPP__
#define __PTF_GAME_HPP__

#include "level.hpp"
#include "editor.hpp"

/**
 * La classe Game gère l'affichage et les objets
 * d'une partie de jeu
 */
class Game : public Level {
private:
    WidgetTimer widget_timer;
    sf::Time next_frame_time;
    bool test_mode;
    std::shared_ptr<View> return_view;

    /**
     * Met à jour les objets du jeu pour
     * qu'ils s'adaptent au nouvel état
     */
    void update();

protected:
    /**
     * Dessine tous les objets, le fond et
     * l'interface de jeu
     */
    virtual void draw();

    /**
     * Traite un événement
     */
    virtual void processEvent(const sf::Event& event);

    /**
     * S'assure que la caméra est centrée sur les joueurs
     */
    void ensureCentered();

public:
    Game(Manager& manager);
    virtual ~Game();

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
     * Mise en mode test : l'appui sur espace renvoie
     * vers l'éditeur donné
     */
    void setTestMode(std::shared_ptr<View> set_return_view);
};

#endif
