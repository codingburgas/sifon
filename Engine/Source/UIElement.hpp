#pragma once

#include "raylib.h"
#include <memory>

class UIElement
{
protected:
	const char* text;
	int fontSize = 18;

	float thickness = 1;

public:
	UIElement() = default;

	virtual ~UIElement() = default;

	virtual void SetText(const char* text);

	Rectangle GetBorder(Vector2 pos, Vector2 size, double rotation);

	void DrawBorder(Rectangle bounds, float width);

	virtual void Draw(Vector2 pos, Vector2 size, double rotation) {};

	template <typename UI>
	static std::shared_ptr<UI> CreateUIElement()
	{
		static_assert(std::is_base_of<UIElement, UI>::value, "UI must inherit UIElement");
		return std::make_shared<UI>();
	}
};

class Label : public UIElement
{
public:
	Label() = default;

	virtual ~Label() = default;

	void Draw(Vector2 pos, Vector2 size, double rotation) override;
};

class Button : public UIElement
{
protected:
	bool is_clicked = false;

public:
	Button() = default;

	virtual ~Button() = default;

	bool GetClicked(Rectangle bounds);

	void Draw(Vector2 pos, Vector2 size, double rotation) override;
};