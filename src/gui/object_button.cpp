#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Renderer.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <cmath>
#include "gui/object_button.hpp"

ObjectButton::Ptr ObjectButton::Create(
    sfg::Image::Ptr image,
    std::shared_ptr<sfg::RadioButtonGroup> group
) {
    auto ptr = Ptr(new ObjectButton);
    ptr->SetImage(image);
    ptr->SetLabel(L"");

    // on définit le groupe de boutons d'objets tel que passé
    // en paramètre, sinon on en crée un nouveau
    if (group) {
        ptr->SetGroup(group);
    } else {
        ptr->SetGroup(sfg::RadioButtonGroup::Create());
    }

    return ptr;
}

const std::string& ObjectButton::GetName() const {
    static const std::string name = "ObjectButton";
    return name;
}

sf::Vector2f ObjectButton::CalculateRequisition() {
    float padding = sfg::Context::Get().GetEngine().GetProperty<float>(
        "Padding", shared_from_this()
    );

    // le widget occupe la taille de l'image de l'objet contenue,
    // plus deux fois le padding spécifié dans le style
    sf::Vector2f child_req = GetChild()->GetRequisition();

    child_req.x += 2 * padding;
    child_req.y += 2 * padding;

    return child_req;
}

std::unique_ptr<sfg::RenderQueue> ObjectButton::InvalidateImpl() const {
    auto queue = std::unique_ptr<sfg::RenderQueue>(new sfg::RenderQueue);

    auto border_color = sfg::Context::Get().GetEngine().GetProperty<sf::Color>(
        "BorderColor", shared_from_this()
    );

    auto background_color = sfg::Context::Get().GetEngine().GetProperty<sf::Color>(
        "BackgroundColor", shared_from_this()
    );

    float padding = sfg::Context::Get().GetEngine().GetProperty<float>(
        "Padding", shared_from_this()
    );

    // si le bouton est actif ou a la souris dessus, on l'entoure
    if (GetState() != sfg::Widget::State::NORMAL) {
        sf::Vector2f position;
        sf::Vector2f size;

        position.x = GetAllocation().width / 2 - GetRequisition().x / 2;
        position.y = GetAllocation().height / 2 - GetRequisition().y / 2;
        size.x = std::min(GetAllocation().width, GetRequisition().x);
        size.y = std::min(GetAllocation().height, GetRequisition().y);

        // si on est en train de cliquer, on décale le fond
        if (GetState() == sfg::Widget::State::ACTIVE) {
            position.x += 1;
            position.y += 1;
        }

		queue->Add(sfg::Renderer::Get().CreatePane(
            position, size, 1.f,
            background_color, border_color, 0
        ));
    }

    // positionnement de l'image du bouton selon l'état
    sf::FloatRect allocation(
        padding, padding, GetAllocation().width - 2 * padding,
        GetAllocation().height - 2 * padding
    );

    if (GetState() == sfg::Widget::State::ACTIVE) {
        allocation.left += 1;
        allocation.top += 1;
    }

    GetChild()->SetAllocation(allocation);
    GetChild()->Invalidate();

    return queue;
}

void ObjectButton::HandleStateChange(sfg::Widget::State old_state) {
    Invalidate();
}
