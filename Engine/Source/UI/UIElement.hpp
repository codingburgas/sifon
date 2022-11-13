#pragma once

#include "raylib.h"

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