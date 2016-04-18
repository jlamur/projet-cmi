#ifndef __SKIZZLE_UTILITY_HPP__
#define __SKIZZLE_UTILITY_HPP__

/**
 * Fonctions utilitaires aux autres classes du jeu
 */
namespace Utility {
    /**
     * Permet d'animer la valeur donnée vers la valeur cible
     * avec la vitesse donnée
     */
    float animateValue(float current, float speed, float goal);
}

#endif
