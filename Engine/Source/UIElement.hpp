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
public:
	Vector2 m_Position{};

	UIElement() = default;

	virtual ~UIElement() = default;

	virtual void Draw() {};
	virtual Vector2 GetSize() = 0;
};

class MainMenuButton : public UIElement
{
private:
	Font m_Font{};

	std::string m_Text{};
	std::function<void()> m_Callback = []() {};
	float m_CenterOffset = 0.f;

	float m_InterpolatedSize = 0.f;
	float m_Time = 0.f;

	CShader m_BloomShader;
	CTexture m_ShaderRenderTexture{};
	
public:
	MainMenuButton(std::string text, float centerOffset, std::function<void()> callback);

	void Draw() override;
	Vector2 GetSize() override;

	~MainMenuButton()
	{
		UnloadFont(m_Font);
	}
	
	// Public static styling of the button
	
	// Font path
	static std::string s_FontPath;
	// Font size
	static float s_FontSize;
	// The size that's being added to the original size for the shader's render texture
	static int s_BufferSizeOffset;

	// The color used for the text glow
	static Color s_GlowColor;
	// Text color
	static Color s_TextColor;

	// How much space is between the text render and the left boundary of the window
	static float s_LeftPadding;

	// Duration for glow animation
	static float s_InterpTime;
};