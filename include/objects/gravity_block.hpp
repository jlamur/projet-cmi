#pragma once
#include <SFML/Graphics.hpp>
#include "../level_data.hpp"
#include "../utility.hpp"
#include "block.hpp"

class Game;

/**
 * Le bloc de gravité est un bloc spécial qui change l'orientation
 * de la gravité dans une direction prédéfinie lorsqu'il est
 * activé par n'importe quel objet. Ce bloc ne peut être utilisé
 * qu'une seule fois par partie
 */
class GravityBlock : public Block {
public:
    /**
     * Identifiant unique du type "bloc de gravité"
     */
    static const unsigned int TYPE_ID;

    /**
     * Charge un bloc de gravité depuis le fichier donné et retourne
     * un pointeur vers cet objet
     */
    static LevelData::ObjectPtr load(std::ifstream& file);

    GravityBlock(sf::Vector2f position = sf::Vector2f());
    virtual LevelData::ObjectPtr clone() const override;
    virtual void draw(sf::RenderWindow&) override;
    virtual unsigned int getTypeId() const override;

protected:
    virtual void activate(Game& game, Object& object) override;
    virtual bool readProperty(unsigned int, std::ifstream& file) override;
    virtual void writeProperties(std::ofstream& file) const override;

    /**
     * La direction de gravité du bloc détermine dans quelle direction
     * se trouvera la gravité du jeu après son activation par un joueur
     * ou un objet
     *
     * (Lecture et écriture.)
     */
    Utility::Direction getGravityDirection() const;
    void setGravityDirection(Utility::Direction);

private:
    Utility::Direction gravity_direction;
    sf::Sprite icon_sprite;
    float opacity;
    bool used;
};
