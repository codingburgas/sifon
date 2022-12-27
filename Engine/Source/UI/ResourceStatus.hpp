#pragma once

#include "UIElement.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"
#include "../Util/graphics.hpp"
#include <array>
#include <sstream>
#include <iomanip>

class ResourceStatus : public UIElement
{
	const char* c_FontPath = "res/fonts/Inter/Inter-Medium.ttf";
	const float c_FontSize = 20.f;
	const float c_OuterPadding = 40.f;
	const float c_InnerPadding = 10.f;

	const float c_ResourceSquareSize = 60.f;

	std::array<CTexture, FACTORS_COUNT> m_Textures{};

	Font m_Font;

	bool m_IsShown = true;

public:
	void ToggleShow()
	{
		m_IsShown = !m_IsShown;
	}

	void Show()
	{
		m_IsShown = true;
	}

	void Hide()
	{
		m_IsShown = false;
	}

	ResourceStatus()
	{
		m_Textures[static_cast<int>(Metadata::MORAL)].LoadFromFile("res/img/moral.png");
		m_Textures[static_cast<int>(Metadata::CLOTHES)].LoadFromFile("res/img/clothes.png");
		m_Textures[static_cast<int>(Metadata::FOOD_WATER)].LoadFromFile("res/img/food.png");
		m_Textures[static_cast<int>(Metadata::GUNS)].LoadFromFile("res/img/guns.png");
		m_Textures[static_cast<int>(Metadata::HOPE)].LoadFromFile("res/img/hope.png");
		m_Textures[static_cast<int>(Metadata::LOYALTY)].LoadFromFile("res/img/loyalty.png");
		m_Textures[static_cast<int>(Metadata::MANPOWER)].LoadFromFile("res/img/manpower.png");
		m_Textures[static_cast<int>(Metadata::MONEY)].LoadFromFile("res/img/money.png");
		m_Font = LoadFontEx(c_FontPath, c_FontSize, nullptr, 0);
	}

	~ResourceStatus()
	{
		UnloadFont(m_Font);
	}

	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);

		if (gameManager->GetPaused() || !m_IsShown) return;

		auto winProbability = gameManager->GetWinProbability(false);

		std::array<int, FACTORS_COUNT> factors{
			static_cast<int>(gameManager->GetCommunityPercent(GameState::CommunityResource::MORAL) * 100.f),
			static_cast<int>(gameManager->GetCommunityPercent(GameState::CommunityResource::LOYALTY) * 100.f),
			gameManager->GetResourceAmount(GameState::Resource::MANPOWER),
			gameManager->GetResourceAmount(GameState::Resource::GUNS),
			static_cast<int>(gameManager->GetCommunityPercent(GameState::CommunityResource::HOPE) * 100.f),
			gameManager->GetMoneyAmount(),
			gameManager->GetResourceAmount(GameState::Resource::FOOD_WATER),
			gameManager->GetResourceAmount(GameState::Resource::CLOTHES)
		};

		Color color = ColorFromHSV(winProbability / 180.f * 180.f, 1.f, .5f);
		DrawRectangle(c_OuterPadding, c_OuterPadding, c_ResourceSquareSize + c_InnerPadding * 2.f + 20.f, c_ResourceSquareSize + c_InnerPadding * 2.f + 20.f, Color{ 56, 56, 56, 149 });
		DrawRectangle(c_OuterPadding + c_InnerPadding, c_OuterPadding + c_InnerPadding,
			c_ResourceSquareSize + 20.f, c_ResourceSquareSize + 20.f, Fade(color, 0.75f));
		DrawRectangleLines(c_OuterPadding, c_OuterPadding, c_ResourceSquareSize + c_InnerPadding * 2.f + 20.f, c_ResourceSquareSize + c_InnerPadding * 2.f + 20.f, WHITE);

		{
			auto text = std::to_string(static_cast<int>(winProbability));
			DrawTextEx(m_Font, text.c_str(),
				{ c_OuterPadding + c_InnerPadding + (c_ResourceSquareSize + 20.f) / 2.f - MeasureTextEx(m_Font, text.c_str(), c_FontSize + 20.f, 1.f).x / 2.f,
				c_OuterPadding + c_InnerPadding + (c_ResourceSquareSize + 20.f) / 2.f - (c_FontSize + 20.f) / 2.f },
				c_FontSize + 20.f, 1.f, WHITE);
		}

		const std::array<int, FACTORS_COUNT> ordering{
			Metadata::MORAL, Metadata::LOYALTY, Metadata::HOPE,
			Metadata::MANPOWER, Metadata::GUNS, Metadata::FOOD_WATER,
			Metadata::CLOTHES, Metadata::MONEY };

		auto offset = Vector2{ c_OuterPadding + c_ResourceSquareSize + c_InnerPadding * 2.f + 20.f, c_OuterPadding };
		for (int i = 0; i < FACTORS_COUNT; i++)
		{
			int j = ordering[i];
			Texture& tex = m_Textures[j].GetTexture();
			DrawRectangle(offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i, offset.y, c_ResourceSquareSize + c_InnerPadding * 2.f, c_ResourceSquareSize + c_InnerPadding * 2.f, Color{ 56, 56, 56, 149 });
			DrawRectangle(offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i + c_InnerPadding, offset.y + c_InnerPadding,
				c_ResourceSquareSize, c_ResourceSquareSize, Fade(DARKGRAY, 0.75f));
			DrawTexturePro(tex, { 0.f, 0.f, static_cast<float>(tex.width), static_cast<float>(tex.height) },
				{ offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i + c_InnerPadding, offset.y + c_InnerPadding,
				c_ResourceSquareSize, c_ResourceSquareSize }, { 0.f, 0.f }, 0.f, Fade(WHITE, 0.45f));
			std::string text;
			if (j == static_cast<int>(Metadata::MORAL) || j == static_cast<int>(Metadata::LOYALTY) || j == static_cast<int>(Metadata::HOPE))
				text = std::to_string(factors[j]) + "%";
			else text = std::to_string(factors[j]);

			DrawTextEx(m_Font, text.c_str(),
				{ offset.x + (c_OuterPadding + c_InnerPadding + c_ResourceSquareSize / 2.f) * (static_cast<float>(i) + .5f) - MeasureTextEx(m_Font, text.c_str(), c_FontSize, 1.f).x / 2.f,
				c_OuterPadding + c_InnerPadding + (c_ResourceSquareSize) / 2.f - (c_FontSize) / 2.f },
				c_FontSize, 1.f, WHITE);
		}

		DrawRectangleLines(offset.x, offset.y,
			(c_ResourceSquareSize + c_InnerPadding * 2.f) * FACTORS_COUNT, c_ResourceSquareSize + c_InnerPadding * 2.f, WHITE);

		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << gameManager->GetTodayWeather();
		std::string tempText = stream.str();

		DrawTextEx(m_Font, std::format("Won/Lost: {}/{}\nTemp: {}C\nM to Advance with 1 month\nD to Advance with 1 day", gameManager->GetWonRevolutions(), gameManager->GetLostRevolutions(), tempText).c_str(), {
			c_OuterPadding, GetScreenHeight() - c_OuterPadding - c_FontSize * 5.5f
			}, c_FontSize, 1.f, WHITE);
	}

	Vector2 GetSize() override
	{
		return { 0, 0 };
	}
};