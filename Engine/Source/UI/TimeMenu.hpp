#pragma once

#include "./UIElement.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"
#include "../Util/graphics.hpp"

class TimeMenu : public UIElement
{
private:
	static float s_Padding;
	static float s_InnerPadding;

	static float s_FontSize;
	static const char* s_FontPath;

	Font m_Font;

	bool m_IsShown = true;

public:
	void ToggleShow()
	{
		m_IsShown = !m_IsShown;
	}

	void Show()
	{
		m_IsShown = true;
	}

	void Hide()
	{
		m_IsShown = false;
	}

	TimeMenu() : m_Font(LoadFontEx(s_FontPath, static_cast<int>(s_FontSize), nullptr, 0))
	{}

	~TimeMenu()
	{
		UnloadFont(m_Font);
	}

	void Draw() override;

	Vector2 GetSize() override { return Vector2{ 0, 0 }; }
};