#pragma once

#include <vector>
#include <string>
#include "./UIBuilder.hpp"
#include "../UI/MainMenuButton.hpp"

class MainMenu : public UIBuilder
{
public:
	MainMenu() : UIBuilder() {}

	void Build(std::shared_ptr<UILayer>& uiLayer) override
	{
		float currentYPos = GetScreenHeight() / 2.f - MainMenuButton::s_FontSize * m_Buttons.size() / 2.f;
		for (auto& [text, callback] : m_Buttons)
		{
			auto button = std::make_shared<MainMenuButton>(text, callback);
			button->m_Position.x = MainMenuButton::s_LeftPadding;
			button->m_Position.y = currentYPos;
			currentYPos += MainMenuButton::s_FontSize;
			uiLayer->PushElement(button);
		}
	}

private:

	struct Button {
		std::string text;
		std::function<void()> callback;
	};

	const std::vector<Button> m_Buttons{
		Button {"PLAY", []() { AppManager::GetInstance()->ChangeScene("res/scenes/character_selection_scene.json"); }},
		Button {"TUTORIAL", []() { OpenURL("https://sifon-docs.gitbook.io/untitled/fundamentals/getting-set-up"); }},
		Button {"EXIT", []() { AppManager::GetInstance()->Quit(); }}
	};
};