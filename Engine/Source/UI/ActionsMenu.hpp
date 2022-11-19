#pragma once

#include "./UIElement.hpp"
#include "./Button.hpp"
#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"
#include "../Managers/EntityManager.hpp"
#include <vector>
#include <memory>
#include <map>
#include <functional>

class ActionButton : public Button
{
public:
	enum class Position
	{
		MOST_LEFT = -1,
		IN_BETWEEN = 0,
		MOST_RIGHT = 1
	};

	ActionButton(std::string text, Position positioning)
		: Button(text, Color{ 56, 56, 56, 149 }), m_Positioning(positioning) {}
	
	~ActionButton() = default;

	void Draw() override
	{
		if (IsMouseInButton())
		{
			m_Transparency = EaseSineInOut(m_Time, s_TransparencyWhenNotHovered, s_TransparencyWhenHovered - s_TransparencyWhenNotHovered, s_InterpTime);
			if (m_Time < s_InterpTime)
				m_Time += GetFrameTime();
		}
		else
		{
			m_Transparency = EaseSineInOut(m_Time, s_TransparencyWhenNotHovered, s_TransparencyWhenHovered - s_TransparencyWhenNotHovered, s_InterpTime);
			if (m_Time > 0.f)
				m_Time -= GetFrameTime();
		}

		auto size = GetSize();
		if (m_Positioning == Position::MOST_LEFT)
		{
			DrawRectangleRec({ m_Position.x + size.y / 2.f, m_Position.y, size.x - size.y / 2.f, size.y },
				Color{ m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
			DrawCircleSector({ m_Position.x + size.y / 2.f, m_Position.y + size.y / 2.f }, size.y / 2.f, 180.f, 360.f, 20,
				Color{ m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
		}
		else if (m_Positioning == Position::MOST_RIGHT)
		{
			DrawRectangleRec({ m_Position.x, m_Position.y, size.x - size.y / 2.f, size.y },
				Color{ m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
			DrawCircleSector({ m_Position.x - size.y / 2.f + size.x, m_Position.y + size.y / 2.f }, size.y / 2.f, 0.f, 180.f, 20,
				Color{ m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
		}
		else if (m_Positioning == Position::IN_BETWEEN)
		{
			DrawRectangleRec({ m_Position.x, m_Position.y, size.x, size.y },
				Color{ m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
		}
		DrawTextEx(m_Font, m_Text.c_str(), Vector2{ m_Position.x + s_Padding.x, m_Position.y + s_Padding.y }, static_cast<float>(s_FontSize), 1.f,
			{ 255, 255, 255, static_cast<unsigned char>(m_Transparency * 255) });
	}

private:
	Position m_Positioning;
};

class ActionsMenu : public UIElement
{
	static float s_BottomPadding;

	struct ButtonData
	{
		std::unique_ptr<ActionButton> m_Button;
		std::function<void()> m_Callback;
	};

	std::vector<ButtonData> m_Buttons;

	void DrawButtons();
	void HandleInput();

public:

	ActionsMenu(std::map<std::string, std::function<void()>> buttons);

	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
		if (gameManager->GetPaused()) return;

		DrawButtons();
		HandleInput();
	}

	Vector2 GetSize() override
	{
		float xSum = 0.f;

		for (auto& button : m_Buttons)
		{
			xSum += button.m_Button->GetSize().x;
		}

		return Vector2{ xSum, m_Buttons.front().m_Button->GetSize().y };
	}
};