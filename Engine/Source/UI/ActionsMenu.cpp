#include "./ActionsMenu.hpp"

ActionsMenu::ActionsMenu(std::map<std::string, std::function<void()>>& buttonMap, float bottomPadding)
{
	m_BottomPadding = bottomPadding;

	std::vector<std::string> buttons{};
	for (auto& [name, callback] : buttonMap) { buttons.push_back(name); }

	if (buttons.size() >= 3)
	{
		m_Buttons.push_back({
			std::make_unique<ActionButton>(buttons.front(), ActionButton::Position::MOST_LEFT),
			buttonMap[buttons.front()]});
		m_Buttons.back().m_Button->m_Position.x = 0.f;
		m_Buttons.back().m_Button->m_Position.y = GetScreenHeight() - m_Buttons.back().m_Button->GetSize().y - m_BottomPadding;

		for (auto it = buttons.begin() + 1; it < buttons.end() - 1; it++)
		{
			auto newButton = std::make_unique<ActionButton>(*it, ActionButton::Position::IN_BETWEEN);

			// position new button
			newButton->m_Position.x = m_Buttons.back().m_Button->m_Position.x + m_Buttons.back().m_Button->GetSize().x;
			m_Buttons.push_back({
				std::move(newButton), buttonMap[*it] });

			// adjust (now new) last button's y axis
			m_Buttons.back().m_Button->m_Position.y = GetScreenHeight() - m_Buttons.back().m_Button->GetSize().y - m_BottomPadding;
		}

		auto newButton = std::make_unique<ActionButton>(buttons.back(), ActionButton::Position::MOST_RIGHT);

		// position new button
		newButton->m_Position.x = m_Buttons.back().m_Button->m_Position.x + m_Buttons.back().m_Button->GetSize().x;
		m_Buttons.push_back({
				std::move(newButton), buttonMap[buttons.back()]});

		// adjust (now new) last button's y axis
		m_Buttons.back().m_Button->m_Position.y = GetScreenHeight() - m_Buttons.back().m_Button->GetSize().y - m_BottomPadding;
	}
	else if (buttons.size() == 2)
	{
		m_Buttons.push_back({
			std::make_unique<ActionButton>(buttons.front(), ActionButton::Position::MOST_LEFT), buttonMap[buttons.front()]});
		m_Buttons.back().m_Button->m_Position.x = 0.f;
		m_Buttons.back().m_Button->m_Position.y = GetScreenHeight() - m_Buttons.back().m_Button->GetSize().y - m_BottomPadding;


		auto newButton = std::make_unique<ActionButton>(buttons.back(), ActionButton::Position::MOST_RIGHT);

		// position new button
		newButton->m_Position.y = GetScreenHeight() - m_Buttons.back().m_Button->GetSize().y - m_BottomPadding;
		newButton->m_Position.x = m_Buttons.back().m_Button->m_Position.x + m_Buttons.back().m_Button->GetSize().x;
		m_Buttons.push_back({
			std::move(newButton), buttonMap[buttons.back()] });
	}

	auto& prevButton = m_Buttons.front();
	prevButton.m_Button->m_Position.x = GetScreenWidth() / 2.f - (prevButton.m_Button->GetSize().x / 2.f) - (GetSize().x - prevButton.m_Button->GetSize().x) / 2.f;

	for (auto it = m_Buttons.begin() + 1; it != m_Buttons.end(); it++)
	{
		(*it).m_Button->m_Position.x = (*(it - 1)).m_Button->m_Position.x + (*(it - 1)).m_Button->GetSize().x;
	}
}

void ActionsMenu::DrawButtons()
{
	DrawRectangleRoundedLines({ m_Buttons.front().m_Button->m_Position.x, m_Buttons.front().m_Button->m_Position.y, GetSize().x, GetSize().y }, Button::s_RoundRadius, 20, 1.f, WHITE);

	// this flag ensures that the first element is skipped
	bool flag = false;
	for (auto& button : m_Buttons)
	{
		button.m_Button->Draw();
		if (flag)
			DrawLine(button.m_Button->m_Position.x, button.m_Button->m_Position.y, button.m_Button->m_Position.x, GetScreenHeight() - m_BottomPadding, WHITE);
		flag = true;
	}
}

void ActionsMenu::HandleInput()
{
	for (auto& button : m_Buttons)
	{
		if (button.m_Button->IsButtonClicked())
			button.m_Callback();
	}
}