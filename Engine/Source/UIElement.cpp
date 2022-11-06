#include "./UIElement.hpp"

#include <iostream>

// MainMenuButton styling and tweaking
std::string MainMenuButton::s_FontPath{ "res/fonts/Inter/Inter-Black.ttf" };
float MainMenuButton::s_FontSize = 100.f;
int MainMenuButton::s_BufferSizeOffset = 40;

Color MainMenuButton::s_GlowColor{ 255, 0, 0, 255 };
Color MainMenuButton::s_TextColor{ 255, 255, 255, 255 };

float MainMenuButton::s_LeftPadding = 60.f;

float MainMenuButton::s_InterpTime = 0.15f;

float EaseSineInOut(float t, float b, float c, float d) { return (-c / 2.0f * (cosf(PI * t / d) - 1.0f) + b); }

Vector2 MainMenuButton::GetSize()
{
	return MeasureTextEx(m_Font, m_Text.c_str(), s_FontSize, 1.f);
}

MainMenuButton::MainMenuButton(std::string text, float centerOffset, std::function<void()> callback)
	: m_Text(text), m_CenterOffset(centerOffset), m_Callback(callback)
{
	// Load font
	m_Font = LoadFontEx(s_FontPath.c_str(), static_cast<int>(s_FontSize), nullptr, 0);
	if (m_Font.texture.id == 0)
		throw std::runtime_error("Font \"" + s_FontPath + "\" did not load properly");

	// Load Bloom Shader
	m_BloomShader.Load("", "res/shaders/bloom.fs");
	if (m_BloomShader.GetShader().id == 0)
		throw std::runtime_error("Shader \"res/shaders/bloom.fs\" did not load properly!");

	m_BloomShader.SetShaderValue("res", GetSize(), SHADER_UNIFORM_VEC2);
	m_ShaderRenderTexture.LoadEmpty({ static_cast<int>(GetSize().x) + s_BufferSizeOffset, static_cast<int>(GetSize().y) + s_BufferSizeOffset});

	m_BloomShader.SetShaderValue("directions", 72.f, SHADER_UNIFORM_FLOAT);
	m_BloomShader.SetShaderValue("quality", 9.f, SHADER_UNIFORM_FLOAT);
	m_BloomShader.SetShaderValue("size", 16.f, SHADER_UNIFORM_FLOAT);
}

void MainMenuButton::Draw()
{
	m_ShaderRenderTexture.BeginDrawingTo();
	DrawTextEx(m_Font, m_Text.c_str(), {s_BufferSizeOffset / 2.f, s_BufferSizeOffset / 2.f}, s_FontSize, 1.f, s_TextColor);
	m_ShaderRenderTexture.EndDrawingTo();

	BeginShaderMode(m_BloomShader.GetShader());
	DrawTexture(m_ShaderRenderTexture.GetRenderTexture().texture, m_Position.x - s_BufferSizeOffset / 2.f, m_Position.y - s_BufferSizeOffset / 2.f, WHITE);
	EndShaderMode();

	DrawTextEx(m_Font, m_Text.c_str(), m_Position, s_FontSize, 1.f, s_TextColor);

	if (CheckCollisionPointRec(GetMousePosition(), { m_Position.x, m_Position.y, GetSize().x, GetSize().y}))
	{
		m_BloomShader.SetShaderValue("size", EaseSineInOut(m_Time, 8.f, 24.f, s_InterpTime), SHADER_UNIFORM_FLOAT);
		if (m_Time < s_InterpTime)
			m_Time += GetFrameTime();
		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			m_Callback();
		}
	}
	else
	{
		m_BloomShader.SetShaderValue("size", EaseSineInOut(m_Time, 8.f, 24.f, s_InterpTime), SHADER_UNIFORM_FLOAT);
		if (m_Time > 0.f)
			m_Time -= GetFrameTime();
	}
}