#pragma once

#include "./UIBuilder.hpp"
#include "../UI/ActionsMenu.hpp"
#include "../UI/PauseMenu.hpp"

#include <functional>
#include <map>

class GameUI : public UIBuilder
{
public:
	void Build(std::shared_ptr<UILayer>& uiLayer) override
	{
		const std::map<std::string, std::function<void()>> actionButtonData{
			{"Revolution", []() {}},
			{"Organisation", []() {}},
			{"Resources", []() {}}
		};

		std::shared_ptr<ActionsMenu> aMenu = std::make_shared<ActionsMenu>(actionButtonData);
		std::shared_ptr<PauseMenu> pMenu = std::make_shared<PauseMenu>();

		uiLayer->PushElement(aMenu);
		uiLayer->PushElement(pMenu);
	}
};