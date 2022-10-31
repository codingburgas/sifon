#pragma once

#include "raylib.h"
#include "./Util/graphics.hpp"
#include <memory>
#include <functional>
#include <stdexcept>

// The UIElement represents a singular part of a collection of elements (a.k.a. UILayer).
// Each UIElement has its own Draw function and handles user input by itself (if needed).
class UIElement
{
protected:
	Vector2 m_Size{};

public:
	Vector2 m_Position{};

	UIElement() = default;

	virtual ~UIElement() = default;

	virtual void Draw() {};
	Vector2 GetSize()
	{
		return m_Size;
	}
};

class MainMenuButton : public UIElement
{
private:
	Font m_Font{};

	std::string m_Text{};
	std::function<void()> m_Callback = []() {};
	float m_CenterOffset = 0.f;

	Shader m_BloomShader;
	CTexture m_ShaderRenderTexture{};
	
public:
	MainMenuButton(std::string text, float centerOffset, std::function<void()> callback)
		: m_Text(text), m_CenterOffset(centerOffset), m_Callback(callback)
	{
		// Load font
		m_Font = LoadFontEx(s_FontPath.c_str(), static_cast<int>(s_FontSize), nullptr, 0);
		if (m_Font.texture.id == 0)
			throw std::runtime_error("Font \"" + s_FontPath + "\" did not load properly");
		m_Size = MeasureTextEx(m_Font, m_Text.c_str(), s_FontSize, 1.f);

		// Load Bloom Shader
		m_BloomShader = LoadShader(nullptr, "res/shaders/bloom.fs");
		SetShaderValue(m_BloomShader, GetShaderLocation(m_BloomShader, "size"), &m_Size, SHADER_UNIFORM_VEC2);
		m_ShaderRenderTexture.LoadEmpty({ static_cast<int>(m_Size.x * 1), static_cast<int>(m_Size.y) });

		if (m_BloomShader.id == 0)
			throw std::runtime_error("Shader \"res/shaders/bloom.fs\" did not load properly!");
	}

	void Draw() override;

	~MainMenuButton()
	{
		UnloadFont(m_Font);
		UnloadShader(m_BloomShader);
	}
	
	// Public static styling of the button
	static std::string s_FontPath;
	static float s_FontSize;

	static Color s_GlowColor;
	static Color s_TextColor;

	static float s_LeftPadding;
};