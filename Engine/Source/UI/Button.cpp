#include "Button.hpp"

Vector2 Button::s_Padding = { 30.f, 10.f };
float Button::s_RoundRadius = 10.f;
float Button::s_TransparencyWhenNotHovered = .7f;
float Button::s_TransparencyWhenHovered = 1.f;
float Button::s_InterpTime = .2f;

float Button::s_FontSize = 40.f;
const char* Button::s_FontPath = "res/fonts/Inter/Inter-ExtraBold.ttf";