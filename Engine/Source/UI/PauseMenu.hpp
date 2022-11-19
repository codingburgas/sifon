#pragma once

#include "./Button.hpp"
#include "./UIElement.hpp"
#include "../Util/graphics.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Managers/AppManager.hpp"
#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"

#include <functional>
#include <unordered_map>

class PauseButton : public Button
{
public:
	PauseButton(std::string text, Color color) : Button(text, color) {}

	virtual Vector2 GetSize() override
	{
		return { 400.f, s_FontSize + s_Padding.y * 2.f };
	}
};

class PauseMenu : public UIElement
{
	const float c_Spacing = 10.f;

	struct ButtonData
	{
		std::unique_ptr<PauseButton> m_Button;
		std::function<void()> m_Callback;
	};

	std::vector<ButtonData> m_Buttons;

	void DrawButtons()
	{
		for (auto& button : m_Buttons)
			button.m_Button->Draw();
	}
	void HandleInput()
	{
		for (auto& button : m_Buttons)
		{
			if (button.m_Button->IsButtonClicked())
				button.m_Callback();
		}
	}

public:
	PauseMenu()
	{
		const std::unordered_map<std::string, std::function<void()>> pauseButtonData{
			{"Resume", [&]() { 
				auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
				gameManager->SetPaused(false);
			}},
			{"Evolution Tree", []() {}},
			{"Save", []() {
				GET_COMPONENT_FROM(
					EntityManager::GetInstance()->GetEntityFromTagName("GameController"),
					GameManager)->SaveSavefile();
			}},
			{"Save & Quit", []() {
				GET_COMPONENT_FROM(
					EntityManager::GetInstance()->GetEntityFromTagName("GameController"),
					GameManager)->SaveSavefile();
				AppManager::GetInstance()->ChangeScene("res/scenes/main_menu_scene.json");
			}}
		};

		float currentYPos = GetScreenHeight() / 2.f - (Button::s_FontSize + Button::s_Padding.y * 2.f) * pauseButtonData.size() / 2.f - c_Spacing * (pauseButtonData.size() - 1);

		for (auto& [text, callback] : pauseButtonData)
		{
			auto button = std::make_unique<PauseButton>(text, GRAY);
			button->m_Position.x = GetScreenWidth() / 2.f - button->GetSize().x / 2.f;
			button->m_Position.y = currentYPos;
			currentYPos += Button::s_FontSize + Button::s_Padding.y * 2.f + c_Spacing;
			m_Buttons.push_back({ std::move(button), callback });
		}
	}

	Vector2 GetSize() override
	{
		return { static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
	}
	
	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
		if (!gameManager->GetPaused()) return;

		HandleInput();
		DrawButtons();
	}
};