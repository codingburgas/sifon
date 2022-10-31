#include "./UIElement.hpp"

#include <iostream>

// MainMenuButton styling and tweaking
std::string MainMenuButton::s_FontPath{ "res/fonts/Inter/Inter-Black.ttf" };
float MainMenuButton::s_FontSize = 100.f;

Color MainMenuButton::s_GlowColor{ 255, 0, 0, 255 };
Color MainMenuButton::s_TextColor{ 255, 255, 255, 255 };

float MainMenuButton::s_LeftPadding = 60.f;

void MainMenuButton::Draw()
{
	//DrawCircleGradient(m_Size.x / 2.f + m_Position.x, m_Size.y / 2.f + m_Position.y, m_Size.y, s_GlowColor, {0, 0, 0, 0});
	//DrawTexture(m_Gradient, m_Position.x, m_Position.y, WHITE);

	m_ShaderRenderTexture.BeginDrawingTo();
	DrawTextEx(m_Font, m_Text.c_str(), {}, s_FontSize, 1.f, s_TextColor);
	m_ShaderRenderTexture.EndDrawingTo();

	//BeginShaderMode(m_BloomShader);
	DrawTexture(m_ShaderRenderTexture.GetRenderTexture().texture, m_Position.x, m_Position.y, WHITE);
	//EndShaderMode();

	if (CheckCollisionPointRec(GetMousePosition(), { m_Position.x, m_Position.y, m_Size.x, m_Size.y }))
	{
		DrawRectangleLines(m_Position.x, m_Position.y, m_Size.x, m_Size.y, WHITE);
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			m_Callback();
		}
	}
}