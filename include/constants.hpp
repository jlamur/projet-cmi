#ifndef __PTF_CONSTANTS_HPP__
#define __PTF_CONSTANTS_HPP__

namespace Constants {
    /**
     * Constante d'attraction. Utilisée dans la formule
     * pour le calcul de l'attraction coulombienne entre
     * deux objets
     */
    static const float ATTRACTION = 2000000;

    /**
     * Constante de déplacement. Définit la quantité de
     * mouvement qui est donnée à un objet lorsqu'il
     * est manipulé manuellement par le joueur
     */
    static const float MOVE = 200;

    /**
     * Constante de gravité. Utilisée dans la formule
     * pour calculer la force de gravité appliquée
     * uniformément vers le bas de la fenêtre sur tous
     * les objets
     */
    static const float GRAVITY = 235;

    /**
     * Durée fixe d'une étape de simulation physique
     */
    static const float PHYSICS_TIME = 1.f / 60;

    /**
     * Correction positionnelle : pourcentage de correction
     * et seuil de correction
     */
    static const float CORRECTION_PERCENTAGE = .2f;
    static const float CORRECTION_THRESHOLD = .05f;

    /**
     * Taille de la grille des blocs en pixels
     */
    static const float GRID = 32;

    /**
     * Activation du débogage (affichage des vecteurs)
     */
    static const bool DEBUG_MODE = false;
}

#endif
