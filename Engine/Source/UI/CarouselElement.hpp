#pragma once

#include "./UIElement.hpp"

#include <string>
#include "reasings.h"
#include "../Util/rtextplus.h"
#include "../Util/graphics.hpp"

class CarouselElement : public UIElement
{
public:
	CarouselElement(std::string imgPath, std::string titleFont, std::string descFont, std::string title, std::string description)
		: m_TitleFont(LoadFontEx(titleFont.c_str(), s_TitleFontSize, nullptr, 0)),
		m_DescFont(LoadFontEx(descFont.c_str(), s_DescFontSize, nullptr, 0)),
		m_Title(title), m_Description(description)
	{
		Image imageData = LoadImage(imgPath.c_str());
		m_Image.LoadFromFile(imgPath.c_str());
		AdjustSize();
		CalculateColors(m_ImageAverage, m_ImageAverageComplementary, imageData);

		m_BlurredImage.LoadEmpty(Vec2i{ static_cast<int>(m_Size.x), static_cast<int>(m_Size.y) });
		CalculateBlurredImage();

		UnloadImage(imageData);
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

private:
	// renderers
	void DrawImage();
	void DrawBackground();
	void DrawText();

	// helper functions
	void AdjustSize();
	void CalculateBlurredImage();
	static void CalculateColors(
		/* out */ Color& average,
		/* out */ Color& averageComplementary,
		/* in */ Image image);

	Vector2 m_Size{};

	CTexture m_Image;
	// used for prerendered blur image at load
	CTexture m_BlurredImage;

	Font m_TitleFont;
	Font m_DescFont;

	std::string m_Title;
	std::string m_Description;

	Color m_ImageAverage;
	Color m_ImageAverageComplementary;
};