#ifndef __PTF_CONSTANTS_HPP__
#define __PTF_CONSTANTS_HPP__

namespace Constants {
    /**
     * Constante d'attraction. Utilisée dans la formule
     * pour le calcul de l'attraction coulombienne entre
     * deux objets
     */
    static constexpr float ATTRACTION = 2000;

    /**
     * Constante de déplacement. Définit la quantité de
     * mouvement qui est donnée à un objet lorsqu'il
     * est manipulé manuellement par le joueur
     */
    static constexpr float MOVE = 100;

    /**
     * Constante de gravité. Utilisée dans la formule
     * pour calculer la force de gravité appliquée
     * uniformément vers le bas de la fenêtre sur tous
     * les objets
     */
    static constexpr float GRAVITY = 200;

    /**
     * Taille de la grille des blocs en pixels
     */
    static constexpr float GRID = 32;

    /**
     * Correction positionnelle : pourcentage de correction
     * et seuil de correction
     */
    static constexpr float CORRECTION_PERCENTAGE = .2f;
    static constexpr float CORRECTION_THRESHOLD = .01f;
}

#endif
