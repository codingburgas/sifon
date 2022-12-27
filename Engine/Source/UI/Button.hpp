#pragma once

#include "./UIElement.hpp"
#include "reasings.h"

#include <string>

class Button : public UIElement
{
public:
	static Vector2 s_Padding;
	static float s_RoundRadius;

	static float s_FontSize;
	static float s_TransparencyWhenNotHovered;
	static float s_TransparencyWhenHovered;
	static float s_InterpTime;
	static const char* s_FontPath;

protected:
	std::string m_Text;
	Font m_Font;
	Color m_Color;
	float m_Transparency = s_TransparencyWhenNotHovered;
	float m_Time = 0.f;

public:
	Button(std::string text, Color color) : m_Text(text), m_Color(color), m_Font(LoadFontEx(s_FontPath, s_FontSize, nullptr, 0))
	{}

	virtual ~Button()
	{
		UnloadFont(m_Font);
	}

	virtual void Draw() override
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
		DrawRectangleRounded({ m_Position.x, m_Position.y, size.x, size.y },
			s_RoundRadius, 10, Color { m_Color.r, m_Color.g, m_Color.b, static_cast<unsigned char>(m_Transparency * 255) });
		DrawTextEx(m_Font, m_Text.c_str(), Vector2{ m_Position.x + s_Padding.x, m_Position.y + s_Padding.y }, static_cast<float>(s_FontSize), 1.f,
			{ 255, 255, 255, static_cast<unsigned char>(m_Transparency * 255) });
	}

	bool IsMouseInButton()
	{
		auto size = GetSize();
		return CheckCollisionPointRec(GetMousePosition(), {m_Position.x, m_Position.y, size.x, size.y});
	}

	bool IsButtonClicked()
	{
		return IsMouseInButton() && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
	}

	virtual Vector2 GetSize() override
	{
		return { MeasureTextEx(m_Font, m_Text.c_str(), s_FontSize, 1.f).x + s_Padding.x * 2.f, s_FontSize + s_Padding.y * 2.f };
	}
};