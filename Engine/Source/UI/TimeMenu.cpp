#include "./TimeMenu.hpp"

float TimeMenu::s_Padding = 40.f;
float TimeMenu::s_InnerPadding = 10.f;

float TimeMenu::s_FontSize = 31;
const char* TimeMenu::s_FontPath = "res/fonts/Inter/Inter-Regular.ttf";

void TimeMenu::Draw()
{
	auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);

	if (gameManager->GetPaused() || !m_IsShown) return;

	if (IsKeyPressed(KEY_D))
		gameManager->AdvanceDay();
	else if (IsKeyPressed(KEY_M))
		gameManager->AdvanceMonth();

	std::string text = std::format("{}.{}.{}", gameManager->GetDay(), gameManager->GetMonth(), gameManager->GetYear());

	auto width = static_cast<float>(GetScreenWidth()), height = static_cast<float>(GetScreenHeight());
	auto textWidth = MeasureTextEx(m_Font, text.c_str(), s_FontSize, 1.f).x;

	// base rectangle
	DrawRectangle(width - textWidth - s_InnerPadding * 2.f - s_Padding, s_Padding,
		textWidth + s_InnerPadding * 2.f, s_FontSize + s_InnerPadding * 2.f, Color{ 56, 56, 56, 195 });

	DrawTextPro(m_Font, text.c_str(), { width - s_Padding - s_InnerPadding, s_Padding + s_InnerPadding },
		{ textWidth, 0.f }, 0.f, s_FontSize, 1.f, WHITE);
	DrawRectangleLinesEx({ width - textWidth - s_InnerPadding * 2.f - s_Padding, s_Padding,
		textWidth + s_InnerPadding * 2.f, s_FontSize + s_InnerPadding * 2.f },
		2.f, WHITE);
}