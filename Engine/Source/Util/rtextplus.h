#pragma once

#include <raylib.h>

//--------------------------------------------------------------------------------------
// Module functions definition
//--------------------------------------------------------------------------------------

// Draw text using font inside rectangle limits with support for text selection
void DrawTextBoxedSelectable(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, int selectStart, int selectLength, Color selectTint, Color selectBackTint);

// Draw text using font inside rectangle limits
void DrawTextBoxed(Font font, const char* text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint);