#ifndef __PTF_CONSTANTS_HPP__
#define __PTF_CONSTANTS_HPP__

#include <SFML/System.hpp>

namespace Constants {
    /**
     * Constante d'attraction. Utilisée dans la formule
     * pour le calcul de l'attraction coulombienne entre
     * deux objets
     */
    static const float ATTRACTION = 500000;

    /**
     * Correction positionnelle : pourcentage de correction
     * et seuil de correction
     */
    static const float CORRECTION_PERCENTAGE = .5f;
    static const float CORRECTION_SLOP = .02f;

    /**
     * Taille de la grille des blocs en pixels
     */
    static const float GRID = 32;
}

#endif
