#include "UIComponent.hpp"

#include "../UI/UIElement.hpp"

void UILayer::OnUpdate()
{
	for (auto& element : m_Elements)
	{
		element->Draw();
	}
}

void UILayer::PushElement(std::shared_ptr<UIElement> element)
{
	m_Elements.push_back(element);
}

void UILayer::ClearElements()
{
	m_Elements.clear();
}