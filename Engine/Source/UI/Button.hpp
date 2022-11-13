#pragma once

#include "./UIElement.hpp"

#include <string>

class Button : public UIElement
{
	static Vector2 m_Padding;
	static float m_RoundRadius;

	static float m_FontSize;
	static const char* m_FontPath;
	
	std::string m_Text;
	Font m_Font;
	Color m_Color;

public:
	Button(std::string text, Color color) : m_Text(text), m_Color(color), m_Font(LoadFontEx(m_FontPath, m_FontSize, nullptr, 0))
	{}

	~Button()
	{
		UnloadFont(m_Font);
	}

	void Draw() override
	{
		auto size = GetSize();
		DrawRectangleRounded({ m_Position.x, m_Position.y, size.x, size.y }, m_RoundRadius, 10, m_Color);
		DrawTextEx(m_Font, m_Text.c_str(), Vector2{ m_Position.x + m_Padding.x, m_Position.y + m_Padding.y }, static_cast<float>(m_FontSize), 1.f, WHITE);
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

	Vector2 GetSize() override
	{
		return { MeasureTextEx(m_Font, m_Text.c_str(), m_FontSize, 1.f).x + m_Padding.x * 2.f, m_FontSize + m_Padding.y * 2.f };
	}
};