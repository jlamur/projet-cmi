#ifndef __SKIZZLE_MANAGER_HPP__
#define __SKIZZLE_MANAGER_HPP__

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>

class State;

/**
 * Gestionnaire principal de tous les états, vues et
 * ressources du jeu. Cette classe agit comme un point
 * d'entrée dans le jeu
 */
class Manager {
private:
    /**
     * Titre de la fenêtre. Le setter de cette propriété
     * met à jour l'interface. Ce titre ne doit pas contenir
     * le préfixe "Skizzle -", qui est automatiquement ajouté
     */
    sf::String title;
    sf::RenderWindow window;

    /**
     * Centralisation des éléments du GUI. Le tableau des
     * widgets contient tous les widgets actuellement instanciés,
     * ce qui permet de cacher les widgets de la vue précédente
     * lors du changement de vue
     */
    sfg::SFGUI sfgui;
    sfg::Desktop desktop;
    std::vector<sfg::Widget::Ptr> widgets;

    /**
     * Détermine si l'événement donné s'est passé à l'intérieur
     * de l'interface et doit être ignoré pour la suite ou non
     */
    bool isInsideGUI(const sf::Event& event);

    /**
     * Chronométrage des temps écoulés entre
     * les frames
     */
    sf::Clock clock;
    sf::Time previous_time;

    /**
     * Gestionnaire d'états. Chaque état modélise un "écran" du
     * jeu. Les états sont empilés par d'autres états lorsqu'ils
     * souhaitent effectuer une transition vers un autre écran,
     * et dépilés pour revenir à l'écran précédent.
     */
    State* previous_state;
    std::stack<std::unique_ptr<State>> states;

public:
    /**
     * Taux idéal de frames par seconde. Cette
     * constante permet de définir "l'objectif" de FPS
     * à atteindre. Ceci évite un FPS trop important qui
     * consommerait trop de ressources
     */
    static const unsigned int FPS;

    /**
     * Temps idéal entre deux frames. Cette
     * constante contient le temps qui devrait idéalement
     * séparer deux frames du jeu. C'est l'inverse de
     * la constande FPS (raccourci)
     */
    static const sf::Time FRAME_TIME;

    /**
     * Taille d'une case de la grille du jeu. Les éléments
     * du jeu peuvent se trouver sur n'importe quel pixel
     * mais sont idéalement alignés (au moins pour les blocs)
     * sur une grille dont le pas est défini par cette constante
     */
    static const float GRID;

    Manager();
    ~Manager();

    /**
     * Démarre la boucle principale du jeu. Cette fonction
     * est appelée une fois que le jeu est totalement
     * initialisé. Elle initialise la pile d'états et
     * commence le dessin des frames
     */
    void start();

    /**
     * Empile l'état donné. Ceci a pour effet de passer
     * à "l'écran" dessiné par l'état donné en paramètre.
     * L'état précédent est gardé en mémoire et il suffit
     * de dépiler l'état pour y revenir.
     */
    void pushState(std::unique_ptr<State> set_state);

    /**
     * Dépile l'état actuel. Ceci a pour effet de revenir
     * à "l'écran" précédent
     */
    void popState();

    /**
     * Renvoie le temps écoulé depuis le démarrage du jeu.
     * Ce temps est utilisé comme référentiel absolu pour
     * calibrer les animations par exemple
     */
    sf::Time getCurrentTime() const;

    /**
     * Renvoie la fenêtre actuellement utilisée pour le dessin
     */
    sf::RenderWindow& getWindow();
    const sf::RenderWindow& getWindow() const;

    /**
     * Ajoute ou supprime un nouveau widget de l'interface.
     * Cet appel est délégué à SFGUI, mais permet de remplir
     * le tableau des widgets du Manager
     */
    void addWidget(sfg::Widget::Ptr widget);
    void removeWidget(sfg::Widget::Ptr widget);

    sf::String getTitle();
    void setTitle(sf::String set_title);

    /**
     * Retourne une vue positionnée telle que le coin supérieur
     * gauche se trouve à (0, 0) et que le zoom est en 1:1
     */
    sf::View getCanonicalView();
};

#endif
