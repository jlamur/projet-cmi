#ifndef __PTF_CONSTANTS_HPP__
#define __PTF_CONSTANTS_HPP__

namespace Constants {
    /**
     * Constante d'attraction. Utilisée dans la formule
     * pour le calcul de l'attraction coulombienne entre
     * deux objets
     */
    static constexpr float ATTRACTION = 2000000;

    /**
     * Constante de déplacement. Définit la quantité de
     * mouvement qui est donnée à un objet lorsqu'il
     * est manipulé manuellement par le joueur
     */
    static constexpr float MOVE = 200;

    /**
     * Constante de gravité. Utilisée dans la formule
     * pour calculer la force de gravité appliquée
     * uniformément vers le bas de la fenêtre sur tous
     * les objets
     */
    static constexpr float GRAVITY = 235;

    /**
     * Taille de la grille des blocs en pixels
     */
    static constexpr float GRID = 32;

    /**
     * Activation du débogage (affichage des vecteurs)
     */
    static constexpr bool DEBUG_MODE = false;

    /**
     * Durée fixe d'une étape de simulation physique
     */
    static constexpr float PHYSICS_TIME = 1.f / 60;

    /**
     * Correction positionnelle : pourcentage de correction
     * et seuil de correction
     */
    static constexpr float CORRECTION_PERCENTAGE = .2f;
    static constexpr float CORRECTION_THRESHOLD = .05f;

    /**
     * Masse par défaut d'un objet
     */
    static constexpr float DEFAULT_MASS = 1.f;

    /**
     * Charge par défaut d'un objet
     */
    static constexpr float DEFAULT_CHARGE = 0.f;

    /**
     * Coefficient de restitution par défaut
     */
    static constexpr float DEFAULT_RESTITUTION = 0.4f;

    /**
     * Couche par défaut d'affichage d'un objet
     * (peut être modifié objet par objet)
     */
    static constexpr int DEFAULT_LAYER = 10;
}

#endif
