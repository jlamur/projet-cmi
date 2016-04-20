#ifndef __SKIZZLE_OBJECT_BUTTON_HPP__
#define __SKIZZLE_OBJECT_BUTTON_HPP__

#include <SFGUI/Image.hpp>
#include <SFGUI/RadioButton.hpp>
#include <SFGUI/RadioButtonGroup.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * Widget personnalisé SFGUI. Il s'agit d'un widget héritant directement
 * des boutons radio et qui affiche l'image donnée sur toute sa surface.
 * Il est utilisé dans la toolbar pour choisir le type d'objet à
 * sélectionner
 *
 * (note: cette classe suit les conventions de nommage de SFGUI, pas
 * celles de Skizzle, pour plus de cohérence)
 */
class ObjectButton : public sfg::RadioButton {
public:
    typedef std::shared_ptr<ObjectButton> Ptr;
    typedef std::shared_ptr<const ObjectButton> PtrConst;

    /**
     * Crée un nouveau bouton d'objet. La convention pour les widgets
     * SFGUI est de protéger le constructeur et de les construire uniquement
     * avec la méthode statique ::Create() qui renvoie un pointeur partagé
     */
    static Ptr Create(
        sfg::Image::Ptr image,
        sfg::RadioButtonGroup::Ptr group = sfg::RadioButtonGroup::Ptr()
    );

    /**
     * Renvoie le nom de l'objet. Est utilisé par le parseur de styles
     * pour obtenir la liste des propriétés s'appliquant au widget
     */
    const std::string& GetName() const override;

protected:
    /**
     * On utilise le constructeur par défaut, mais on est obligés de le
     * déclarer explicitement pour qu'il soit protégé
     */
    ObjectButton() = default;

    /**
     * Recalcule la géométrie du widget
     */
    std::unique_ptr<sfg::RenderQueue> InvalidateImpl() const override;

    /**
     * Recalcule la place occupée par le widget
     */
    sf::Vector2f CalculateRequisition() override;
};

#endif
