#include "UIComponent.hpp"
#include "TransformComponent2D.hpp"

#include "../UIElement.hpp"

void UILayer::OnUpdate()
{
	for (auto& element : m_Elements)
	{
		element->Draw();
	}
}

void UILayer::PushElement(std::shared_ptr<UIElement> element)
{
	m_Elements.insert(element);
}