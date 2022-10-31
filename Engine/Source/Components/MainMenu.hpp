#pragma once

#include <map>
#include <string>
#include "./UIBuilder.hpp"

class MainMenu : public UIBuilder
{
public:
	MainMenu() : UIBuilder() {}

	void Build(std::shared_ptr<UILayer>& uiLayer) override
	{
		float currentYPos = GetScreenHeight() / 2.f - MainMenuButton::s_FontSize * m_ButtonMap.size() / 2.f;
		for (auto& [text, callback] : m_ButtonMap)
		{
			auto button = std::make_shared<MainMenuButton>(text, 0.f, callback);
			button->m_Position.x = MainMenuButton::s_LeftPadding;
			button->m_Position.y = currentYPos;
			currentYPos += MainMenuButton::s_FontSize;
			uiLayer->PushElement(button);
		}
	}

private:
	const std::map<std::string, std::function<void()>> m_ButtonMap
	{
		std::pair<std::string, std::function<void()>> {"PLAY", []() { AppManager::GetInstance()->ChangeScene("play_menu_scene.json"); }},
		std::pair<std::string, std::function<void()>> {"MODES", []() { AppManager::GetInstance()->ChangeScene("modes_menu_scene.json"); }},
		std::pair<std::string, std::function<void()>> {"SETTINGS", []() { AppManager::GetInstance()->ChangeScene("settings_menu_scene.json"); }}
	};
};