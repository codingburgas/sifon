#pragma once

#include "./UIElement.hpp"

#include <string>
#include "reasings.h"
#include "../Util/rtextplus.h"
#include "../Util/graphics.hpp"
#include "../Components/MappackMetadataStore.hpp"
#include <sstream>
#include <iomanip>

class StatsViewer : public UIElement
{
	std::array<CTexture, FACTORS_COUNT> m_Textures{};
	std::array<float, FACTORS_COUNT> m_Factors{};
	const float c_FontSize = 20.f;
	const float c_ResourceSquareSize = 60.f;
	const float c_InnerPadding = 10.f;
	Font m_Font;
public:
	Vector2 m_Offset = {};

	StatsViewer(std::string fontPath, std::map<Metadata::Factor, float> factors)
	{
		m_Font = LoadFontEx(fontPath.c_str(), static_cast<int>(c_FontSize), 0, 0);

		for (auto& [type, value] : factors)
			m_Factors[type] = value;

		m_Textures[static_cast<int>(Metadata::MORAL)].LoadFromFile("res/img/moral.png");
		m_Textures[static_cast<int>(Metadata::CLOTHES)].LoadFromFile("res/img/clothes.png");
		m_Textures[static_cast<int>(Metadata::FOOD_WATER)].LoadFromFile("res/img/food.png");
		m_Textures[static_cast<int>(Metadata::GUNS)].LoadFromFile("res/img/guns.png");
		m_Textures[static_cast<int>(Metadata::HOPE)].LoadFromFile("res/img/hope.png");
		m_Textures[static_cast<int>(Metadata::LOYALTY)].LoadFromFile("res/img/loyalty.png");
		m_Textures[static_cast<int>(Metadata::MANPOWER)].LoadFromFile("res/img/manpower.png");
		m_Textures[static_cast<int>(Metadata::MONEY)].LoadFromFile("res/img/money.png");
	}

	~StatsViewer()
	{
		UnloadFont(m_Font);
	}

	void Draw() override
	{
		auto offset = Vector2 { m_Position.x + m_Offset.x, m_Position.y + m_Offset.y };
		for (int i = 0; i < FACTORS_COUNT; i++)
		{
			const std::array<int, FACTORS_COUNT> ordering{
				Metadata::MORAL, Metadata::LOYALTY, Metadata::HOPE,
				Metadata::MANPOWER, Metadata::GUNS, Metadata::FOOD_WATER,
				Metadata::CLOTHES, Metadata::MONEY };

			int j = ordering[i];
			Texture& tex = m_Textures[j].GetTexture();
			DrawRectangle(static_cast<int>(offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i),
				static_cast<int>(offset.y), static_cast<int>(c_ResourceSquareSize + c_InnerPadding * 2.f),
				static_cast<int>(c_ResourceSquareSize + c_InnerPadding * 2.f), Color{ 56, 56, 56, 149 });
			DrawRectangle(static_cast<int>(offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i + c_InnerPadding),
				static_cast<int>(offset.y + c_InnerPadding), static_cast<int>(c_ResourceSquareSize),
				static_cast<int>(c_ResourceSquareSize), Fade(DARKGRAY, 0.75f));
			DrawTexturePro(tex, { 0.f, 0.f, static_cast<float>(tex.width), static_cast<float>(tex.height) },
				{ offset.x + (c_ResourceSquareSize + c_InnerPadding * 2.f) * i + c_InnerPadding, offset.y + c_InnerPadding,
				c_ResourceSquareSize, c_ResourceSquareSize }, { 0.f, 0.f }, 0.f, Fade(WHITE, 0.45f));

			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << m_Factors[j];
			std::string text = stream.str() + "x";

			DrawTextEx(m_Font, text.c_str(),
				{ offset.x + (c_InnerPadding * 2.f + c_ResourceSquareSize) * (static_cast<float>(i) + .5f) - MeasureTextEx(m_Font, text.c_str(), c_FontSize, 1.f).x / 2.f,
				c_InnerPadding + (c_ResourceSquareSize) / 2.f - (c_FontSize) / 2.f + m_Offset.y },
				c_FontSize, 1.f, WHITE);
		}
	}

	Vector2 GetSize() override
	{
		return {};
	}
};

class CarouselElement : public UIElement
{
public:
	CarouselElement(std::string imgPath, std::string titleFont, std::string descFont, std::string title, std::string description, std::map<Metadata::Factor, float> boosts)
		: m_TitleFont(LoadFontEx(titleFont.c_str(), s_TitleFontSize, nullptr, 0)),
		m_DescFont(LoadFontEx(descFont.c_str(), s_DescFontSize, nullptr, 0)),
		m_Title(title), m_Description(description),
		m_StatsViewer(std::string{ "res/fonts/Inter/Inter-Medium.ttf" }, boosts)
	{
		Image imageData = LoadImage(imgPath.c_str());
		m_Image.LoadFromFile(imgPath.c_str());
		AdjustSize();

		m_BlurredImage.LoadEmpty(Vec2i{ static_cast<int>(m_Size.x), static_cast<int>(m_Size.y) });
		CalculateBlurredImage();

		UnloadImage(imageData);

		m_StatsViewer.m_Offset.x = m_Position.x + m_Size.x / 3.f + s_InnerPadding * 2.f;
		m_StatsViewer.m_Offset.y = GetSize().y - s_InnerPadding - 80.f;
	}

	~CarouselElement()
	{
		UnloadFont(m_TitleFont);
		UnloadFont(m_DescFont);
	}

	virtual Vector2 GetSize() override;
	virtual void Draw() override;

	void SetTitle(std::string);
	void SetDescription(std::string);

	// styling
	static float s_SizePercentage;
	static float s_InnerPadding;

	static int s_TitleFontSize;
	static int s_DescFontSize;
	
	StatsViewer m_StatsViewer;

private:
	// renderers
	void DrawImage();
	void DrawBackground();
	void DrawText();

	// helper functions
	void AdjustSize();
	void CalculateBlurredImage();

	Vector2 m_Size{};

	CTexture m_Image;
	// used for prerendered blur image at load
	CTexture m_BlurredImage;

	Font m_TitleFont;
	Font m_DescFont;

	std::string m_Title;
	std::string m_Description;
};