#pragma once

#pragma once

#include "./UIElement.hpp"
#include "../Components/GameManager.hpp"
#include "../Managers/EntityManager.hpp"
#include "../UI/Button.hpp"
#include <map>
#include "raylib.h"

class EventSummary : public UIElement
{
	const float c_TitleFontSize = 80.f;
	const float c_DescriptionFontSize = 30.f;

	const std::string c_TitleFontPath = "res/fonts/Inter/Inter-Black.ttf";
	const std::string c_SubtitleFontPath = "res/fonts/Inter/Inter-Bold.ttf";
	const std::string c_DescriptionFontPath = "res/fonts/Inter/Inter-Medium.ttf";

	const float c_InnerPadding = 30.f;
	const float c_SizePercent = .30f;

	const Color c_BackgroundColor = Color{ 7, 17, 28, 255 };

	Font m_TitleFont, m_SubtitleFont, m_DescriptionFont;
	bool m_IsShown = false;

	Button m_CloseButton;

public:
	EventSummary()
		: m_TitleFont(LoadFontEx(c_TitleFontPath.c_str(), static_cast<int>(c_TitleFontSize), 0, 0)),
		m_SubtitleFont(LoadFontEx(c_SubtitleFontPath.c_str(), static_cast<int>(c_TitleFontSize), 0, 0)),
		m_DescriptionFont(LoadFontEx(c_DescriptionFontPath.c_str(), static_cast<int>(c_DescriptionFontSize), 0, 0)),
		m_CloseButton("close", Color{ 255, 65, 65, 255 }) {};

	void Show()
	{
		m_IsShown = true;
		m_CloseButton.m_Position.x = GetScreenWidth() - GetSize().x + c_InnerPadding;
		m_CloseButton.m_Position.y = GetScreenHeight() - c_InnerPadding - m_CloseButton.GetSize().y;
	}

	void Hide()
	{
		m_IsShown = false;
	}

	void ToggleShow()
	{
		m_IsShown = !m_IsShown;
	}

	~EventSummary()
	{
		UnloadFont(m_TitleFont);
		UnloadFont(m_SubtitleFont);
		UnloadFont(m_DescriptionFont);
	}

	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
		auto& state = gameManager->GetState();
		if (!m_IsShown || gameManager->GetPaused()) return;

		DrawRectangle(GetScreenWidth() - static_cast<int>(GetSize().x), 0, static_cast<int>(GetSize().x), static_cast<int>(GetSize().y), c_BackgroundColor);

		auto offset = Vector2 { static_cast<float>(GetScreenWidth()) - GetSize().x + c_InnerPadding, c_InnerPadding };
		{
			DrawTextEx(m_TitleFont, "Revolution", offset, c_TitleFontSize, 1.f, WHITE);
			if (state.m_LastRevolutionWon)
				DrawTextEx(m_SubtitleFont, "Won!", { offset.x, offset.y + c_TitleFontSize }, c_TitleFontSize, 1.f, GREEN);
			else DrawTextEx(m_SubtitleFont, "Lost!", { offset.x, offset.y + c_TitleFontSize }, c_TitleFontSize, 1.f, RED);

			DrawLineEx({ static_cast<float>(GetScreenWidth()) - GetSize().x, offset.y + c_InnerPadding + c_TitleFontSize * 2.f },
				{ static_cast<float>(GetScreenWidth()), offset.y + c_InnerPadding + c_TitleFontSize * 2.f }, 8.f, WHITE);
			DrawLineEx({ static_cast<float>(GetScreenWidth()) - GetSize().x, 0.f }, { static_cast<float>(GetScreenWidth()) - GetSize().x, static_cast<float>(GetScreenHeight()) }, 8.f, WHITE);
		}

		offset.y += c_InnerPadding * 2.f + c_TitleFontSize * 2.f;

		{
			DrawTextEx(m_SubtitleFont, "DEATHS", offset, c_TitleFontSize, 1.f, Color{ 135, 55, 184, 255 });
			offset.y += c_TitleFontSize + c_InnerPadding / 2.f;
			offset.x += c_InnerPadding;
			DrawTextEx(m_DescriptionFont,
				std::format("- {} people died\n- Out of {} people", abs(state.m_LastRevolutionAlters.m_Manpower),
					state.m_Resources.m_Manpower + abs(state.m_LastRevolutionAlters.m_Manpower)).c_str(), offset, c_DescriptionFontSize, 1.f, WHITE);
		}

		offset.y += c_DescriptionFontSize * 2.f + c_InnerPadding * 2.f;
		offset.x -= c_InnerPadding;

		{
			DrawTextEx(m_SubtitleFont, "RESOURCES", offset, c_TitleFontSize, 1.f, Color{ 73, 133, 198, 255 });
			offset.y += c_TitleFontSize + c_InnerPadding / 2.f;
			offset.x += c_InnerPadding;

			auto& resources = state.m_Resources;

			std::vector<int> resVec = {
				resources.m_Clothes + abs(state.m_LastRevolutionAlters.m_Clothes),
				resources.m_Food + abs(state.m_LastRevolutionAlters.m_Food),
				resources.m_GunAmount + abs(state.m_LastRevolutionAlters.m_GunAmount),
				resources.m_Money + abs(state.m_LastRevolutionAlters.m_Money)
			};
			std::string result;
			result += std::format("- {}% clothes depleted\n", resources.m_Clothes / resVec[0]);
			result += std::format("- {}% food depleted\n", resources.m_Food / resVec[1]);
			result += std::format("- {}% guns depleted\n", resources.m_GunAmount / resVec[2]);

			DrawTextEx(m_DescriptionFont,
				result.c_str(), offset, c_DescriptionFontSize, 1.f, RED);

			offset.y += c_DescriptionFontSize * 4.5f;

			if (state.m_LastRevolutionAlters.m_Money > 0.f)
			{
				DrawTextEx(m_DescriptionFont, std::format(
					"- {}% money increase", static_cast<int>(100.f * static_cast<float>(resources.m_Money) / static_cast<float>(resources.m_Money + abs(state.m_LastRevolutionAlters.m_Money)))).c_str(),
					offset, c_DescriptionFontSize, 1.f, GREEN);
			}
			else
			{
				DrawTextEx(m_DescriptionFont, std::format(
					"- {}% money decrease", static_cast<int>(100.f * static_cast<float>(resources.m_Money) / static_cast<float>(resources.m_Money + abs(state.m_LastRevolutionAlters.m_Money)))).c_str(),
					offset, c_DescriptionFontSize, 1.f, RED);
			}
		}

		if (m_CloseButton.IsButtonClicked())
			Hide();

		m_CloseButton.Draw();
	}

	Vector2 GetSize() override {
		return { static_cast<float>(GetScreenWidth()) * c_SizePercent, static_cast<float>(GetScreenHeight()) };
	}
};