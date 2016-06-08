#pragma once
#include <SFML/Window.hpp>

/**
 * Fonctions utilitaires aux autres classes du jeu
 */
namespace Utility {
    enum class Direction {NORTH, EAST, SOUTH, WEST};
    enum class Modifier {CONTROL, ALT, SHIFT, SYSTEM};

    /**
     * Permet d'animer la valeur donnée (current) vers la valeur
     * cible (goal) avec la vitesse donnée (speed)
     */
    float animateValue(float current, float speed, float goal);

    /**
     * Vérifie si la touche donnée en paramètre est actuellement
     * pressée. Si un modificateur est passé en paramètre, vérifie
     * si le modifcateur de gauche OU de droite est pressé
     */
    bool isKeyPressed(sf::Keyboard::Key key);
    bool isKeyPressed(Modifier modifier);
}
