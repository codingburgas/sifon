#pragma once

#include "./UI/UIElement.hpp"
#include "./UIComponent.hpp"
#include "./Component.hpp"
#include "../Managers/ECSManager.hpp"
#include "../Managers/AppManager.hpp"
#include "../Managers/EntityManager.hpp"

// The UIBuilder is an interface for classes to create a UILayer and
// populate them with their own UIElement(s).
class UIBuilder : public Component
{
public:
	DEFINE_COMPONENT(UIBuilder);

	void OnCreate() override
	{
		auto ui = ECS::CreateComponent<UILayer>();
		Build(ui);
		boundEntity.lock()->AddComponent(ui);
		boundEntity.lock()->RemoveComponent(ComponentName());
	}

	virtual void Build(std::shared_ptr<UILayer>& uiLayer) = 0;
};