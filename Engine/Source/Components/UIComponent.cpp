#include "UIComponent.hpp"
#include "TransformComponent2D.hpp"

#include "../UIElement.hpp"

void UIComponent::OnCreate()
{
	if (type == "Label")
	{
		element = UIElement::CreateUIElement<Label>();
	}
	else if (type == "Button")
	{
		element = UIElement::CreateUIElement<Button>();
	}
	else return;

	element->SetText(text.c_str());
}

void UIComponent::OnUpdate()
{
	std::shared_ptr<TransformComponent2D> transform = GET_COMPONENT(TransformComponent2D);

	element->Draw(
		transform->Position,
		{ 120 * transform->Scale.x, 60 * transform->Scale.y },
		transform->Rotation
	);
}

void UIComponent::OnDestroy() {}