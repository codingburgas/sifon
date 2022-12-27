#include "./CarouselElement.hpp"

// CarouselElement styling
float CarouselElement::s_SizePercentage = 0.725f;
float CarouselElement::s_InnerPadding = 40.f;

int CarouselElement::s_TitleFontSize = 87;
int CarouselElement::s_DescFontSize = 32;

// code for fitting image within the square without stretching and leaving no empty spaces as possible
Vector2 FitRectangleInRectangle(Vector2 inner, Vector2 outer)
{
	float innerAspectRatio = inner.x / inner.y;
	float outerAspectRatio = outer.x / outer.y;
	float scale;
	if (innerAspectRatio >= outerAspectRatio)
		scale = inner.y / outer.y;
	else
		scale = inner.x / outer.x;
	return { scale * outer.x, scale * outer.y };
}

void CarouselElement::DrawBackground()
{
	Vector2& destRes = m_Size;
	Vector2 srcRes = { FitRectangleInRectangle(Vector2{static_cast<float>(m_Image.GetTexture().width), static_cast<float>(m_Image.GetTexture().height)}, destRes)};

	DrawTexture(m_BlurredImage.GetRenderTexture().texture, static_cast<int>(m_Position.x), static_cast<int>(m_Position.y), WHITE);
	DrawRectangleLinesEx({ m_Position.x, m_Position.y, m_Size.x, m_Size.y }, 4.f, WHITE);
}

// image draw code
// image rectangle takes up 30% of m_Size - s_InnerPadding * 3
void CarouselElement::DrawImage()
{
	Vector2 destRes{ (m_Size.x - s_InnerPadding * 3.f) / 3.f, m_Size.y - s_InnerPadding * 2.f };
	Vector2 srcRes{ FitRectangleInRectangle(Vector2{static_cast<float>(m_Image.GetTexture().width), static_cast<float>(m_Image.GetTexture().height)}, destRes)};
	DrawTexturePro(m_Image.GetTexture(), Rectangle{
			m_Image.GetTexture().width / 2.f - srcRes.x / 2.f, 0, srcRes.x, srcRes.y
		}, Rectangle{
			m_Position.x + s_InnerPadding, m_Position.y + s_InnerPadding,
			destRes.x, destRes.y
		}, {}, 0.f, WHITE);
}

void CarouselElement::DrawText()
{
	DrawTextEx(m_TitleFont, m_Title.c_str(), Vector2{ m_Position.x + m_Size.x / 3.f + s_InnerPadding * 2.f, m_Position.y + s_InnerPadding }, s_TitleFontSize, 1.f, WHITE);
	DrawLineEx(Vector2{ m_Position.x + m_Size.x / 3.f + s_InnerPadding * 2.f, m_Position.y + s_InnerPadding + s_TitleFontSize },
		Vector2{ m_Position.x + m_Size.x / 3.f + s_InnerPadding * 2.f + MeasureTextEx(m_TitleFont, m_Title.c_str(), s_TitleFontSize, 1.f).x, m_Position.y + s_InnerPadding + s_TitleFontSize },
		4.f, WHITE);

	DrawTextBoxed(m_DescFont, m_Description.c_str(), Rectangle{
			m_Position.x + m_Size.x / 3.f + s_InnerPadding * 2.f, m_Position.y + s_InnerPadding * 2.f + s_TitleFontSize,
			m_Size.x / 1.5f - s_InnerPadding * 3.f,
			m_Size.y + s_InnerPadding - s_TitleFontSize
		}, static_cast<float>(s_DescFontSize), 1.f, true, WHITE);

	m_StatsViewer.Draw();
}

void CarouselElement::Draw()
{
	DrawBackground();
	DrawImage();
	DrawText();
}

Vector2 CarouselElement::GetSize()
{
	if (IsWindowResized())
	{
		AdjustSize();
		m_BlurredImage.LoadEmpty(Vec2i{ static_cast<int>(m_Size.x), static_cast<int>(m_Size.y) });
		CalculateBlurredImage();
	}

	return m_Size;
}

void CarouselElement::AdjustSize()
{
	Vector2 winRes = { GetScreenWidth() * 1.f, GetScreenHeight() * 1.f };
	m_Size = { winRes.x - winRes.x * (1.f - s_SizePercentage), winRes.y - winRes.y * (1.f - s_SizePercentage) };
}

void CarouselElement::CalculateBlurredImage()
{
	CShader blurShader{};
	blurShader.Load("", "res/shaders/bloom.fs");
	blurShader.SetShaderValue("res", m_Size, SHADER_UNIFORM_VEC2);
	blurShader.SetShaderValue("size", 48.f, SHADER_UNIFORM_FLOAT);

	Vector2 destRes{ (m_Size.x) / 3.f - s_InnerPadding, m_Size.y - s_InnerPadding };
	Vector2 srcRes{ FitRectangleInRectangle(Vector2{static_cast<float>(m_Image.GetTexture().width), static_cast<float>(m_Image.GetTexture().height)}, destRes)};

	RenderTexture imageBuffer = LoadRenderTexture(m_Size.x, m_Size.y);

	BeginTextureMode(imageBuffer);
	ClearBackground(BLACK);
	ClearBackground(Color{ 80, 80, 80, 127 });
	DrawTexturePro(m_Image.GetTexture(), Rectangle{
			m_Image.GetTexture().width / 2.f - srcRes.x / 2.f, 0, srcRes.x, srcRes.y
		}, Rectangle{
			m_Position.x + s_InnerPadding / 2.f, m_Position.y + s_InnerPadding / 2.f,
			destRes.x + s_InnerPadding, destRes.y
		}, {}, 0.f, WHITE);
	EndTextureMode();

	BeginTextureMode(m_BlurredImage.GetRenderTexture());
	ClearBackground(BLACK);
	BeginShaderMode(blurShader.GetShader());
	DrawTexture(imageBuffer.texture, 0, 0, WHITE);
	EndShaderMode();
	EndTextureMode();

	UnloadRenderTexture(imageBuffer);
}

void CarouselElement::SetTitle(std::string title)
{
	m_Title = title;
}

void CarouselElement::SetDescription(std::string description)
{
	m_Description = description;
}