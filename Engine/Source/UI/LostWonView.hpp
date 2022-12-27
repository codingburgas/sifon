#pragma once

#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"

#include "../Managers/EntityManager.hpp"
#include "./UIElement.hpp"


class LostWonView : public UIElement
{
	const std::string c_TitleFontPath = "res/fonts/Inter/Inter-ExtraBold.ttf";
	const float c_TitleFontSize = 200.f;
	
	const std::string c_DescriptionFontPath = "res/fonts/Inter/Inter-Regular.ttf";
	const float c_DescriptionFontSize = 50.f;

	Font m_TitleFont;
	Font m_DescriptionFont;

	std::function<void()> m_HideAllMethod;

public:
	LostWonView(std::function<void()> hideAllMethod)
		: m_TitleFont(LoadFontEx(c_TitleFontPath.c_str(), static_cast<int>(c_TitleFontSize), 0, 0)), 
		  m_DescriptionFont(LoadFontEx(c_DescriptionFontPath.c_str(), c_DescriptionFontSize, 0, 0)), m_HideAllMethod(hideAllMethod) {}
	
	~LostWonView()
	{
		UnloadFont(m_TitleFont);
		UnloadFont(m_DescriptionFont);
	}

	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"),
			GameManager);
		auto status = gameManager->GetGameStatus();
		auto winRes = Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };

		if (status.m_Status == GameManager::WinningStatus::LOST)
		{
			if (IsKeyPressed(KEY_ESCAPE)) AppManager::GetInstance()->ChangeScene("res/scenes/main_menu_scene.json");
			m_HideAllMethod();
			m_HideAllMethod = []() {};
			DrawTextEx(m_TitleFont, "YOU LOST",
				{ (GetScreenWidth() - MeasureTextEx(m_TitleFont, "YOU LOST", c_TitleFontSize, 1.f).x) / 2.f,
				(GetScreenHeight() - c_TitleFontSize) / 2.f }, c_TitleFontSize, 1.f, RED);
			DrawTextEx(m_DescriptionFont, status.m_Reason.c_str(),
				{ (GetScreenWidth() - MeasureTextEx(m_DescriptionFont, status.m_Reason.c_str(), c_DescriptionFontSize, 1.f).x) / 2.f,
				GetScreenHeight() - c_DescriptionFontSize - 10.f },
				c_DescriptionFontSize, 1.f, RED);
		}
		else if (status.m_Status == GameManager::WinningStatus::WON)
		{
			if (IsKeyPressed(KEY_ESCAPE)) AppManager::GetInstance()->ChangeScene("res/scenes/main_menu_scene.json");
			m_HideAllMethod();
			m_HideAllMethod = []() {};
			DrawTextEx(m_TitleFont, "YOU WON",
				{ (GetScreenWidth() - MeasureTextEx(m_TitleFont, "YOU WON", c_TitleFontSize, 1.f).x) / 2.f,
				(GetScreenHeight() - c_TitleFontSize) / 2.f }, c_TitleFontSize, 1.f, GREEN);
			DrawTextEx(m_DescriptionFont, status.m_Reason.c_str(),
				{ (GetScreenWidth() - MeasureTextEx(m_DescriptionFont, status.m_Reason.c_str(), c_DescriptionFontSize, 1.f).x) / 2.f,
				GetScreenHeight() - c_DescriptionFontSize - 10.f },
				c_DescriptionFontSize, 1.f, GREEN);
		}
	}

	Vector2 GetSize() override
	{
		return { 0.f, 0.f };
	}
};