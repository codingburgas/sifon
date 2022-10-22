#include "./UIElement.hpp"

#include <iostream>

void UIElement::SetText(const char* text)
{
	this->text = text;
}

Rectangle UIElement::GetBorder(Vector2 pos, Vector2 size, double rotation)
{
	const Vector2 offset = { 60, 30 };

	Vector2 borderSize = {
		MeasureText(text, fontSize) + offset.x,
		fontSize + offset.y
	};

	if (borderSize.x < size.x && borderSize.y < size.y)
		borderSize = size;

	Rectangle bounds = {
		pos.x - offset.x / 2,
		pos.y - offset.y / 2,
		borderSize.x,
		borderSize.y
	};

	return bounds;
}

void UIElement::DrawBorder(Rectangle bounds, float width)
{
	DrawRectangleLinesEx(
		bounds,
		width,
		Color(BLACK)
	);
}

void Label::Draw(Vector2 pos, Vector2 size, double rotation)
{
	Rectangle bounds = GetBorder(pos, size, rotation);

	DrawBorder(bounds, thickness);

	DrawText(
		text,
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		fontSize,
		Color(BLACK)
	);
}

bool Button::GetClicked(Rectangle bounds)
{
	Vector2 mousePos = GetMousePosition();

	return CheckCollisionPointRec(mousePos, bounds) && IsMouseButtonPressed(0);
}

void Button::Draw(Vector2 pos, Vector2 size, double rotation)
{
	Rectangle bounds = GetBorder(pos, size, rotation);

	DrawBorder(bounds, thickness);
	

	is_clicked = GetClicked(bounds);
	
	if (is_clicked)
		printf("UI: Button is clicked\n");


	DrawText(
		text,
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		fontSize,
		Color(BLACK)
	);
}