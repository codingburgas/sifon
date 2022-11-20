#include "./Carousel.hpp"
#include "./CarouselElement.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Components/Component.hpp"
#include "../Components/GameManager.hpp"

// Carousel styling
float Carousel::s_InterpTime = .6f;
float Carousel::s_Spacing = 90.f;
Vector2 Carousel::s_NavButtonSize = { 80.f, 80.f };

void Carousel::StartAnimation()
{
	AdjustCamera(m_TargetCamera, m_SelectedElement);
	m_InAnimation = true;
	m_OldCamera = m_Camera;
	m_Time = 0.f;
}

void Carousel::StopAnimation()
{
	m_InAnimation = false;
}

void Carousel::HandleSlidingInput()
{
	if (!(!m_BackButton.IsMouseInButton() && !m_StartButton.IsMouseInButton()))
		return;
	
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		StopAnimation();
		m_Camera.target.x -= GetMouseDelta().x;
	}
	else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		if (m_Camera.target.x > ((CarouselElement::s_SizePercentage * GetScreenWidth() + s_Spacing) * (m_SelectedElement + 1) - s_Spacing * 3.f))
		{
			if (m_SelectedElement + 1 <= m_Elements.size() - 1)
			{
				m_SelectedElement++;
				StartAnimation();
			}
			else StartAnimation();
		}
		else if (m_Camera.target.x < ((CarouselElement::s_SizePercentage * GetScreenWidth() + s_Spacing) * (m_SelectedElement)+s_Spacing * 2.f))
		{
			if (m_SelectedElement - 1 >= 0)
			{
				m_SelectedElement--;
				StartAnimation();
			}
			else StartAnimation();
		}
		else StartAnimation();
	}
}

void Carousel::HandleKeyboardInput()
{
	if (IsKeyPressed(KEY_LEFT) && (m_SelectedElement - 1 >= 0))
	{
		m_SelectedElement--;
		StartAnimation();
	}
	else if (IsKeyPressed(KEY_RIGHT) && (m_SelectedElement + 1 <= m_Elements.size() - 1))
	{
		m_SelectedElement++;
		StartAnimation();
	}
}

void Carousel::HandleInput()
{
	if (IsWindowResized())
	{
		AdjustCamera(m_Camera, m_SelectedElement);
		AdjustElements(m_Elements);
		AdjustButtons(m_StartButton, m_BackButton);
	}

	HandleSlidingInput();

	if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		HandleKeyboardInput();

	if (m_BackButton.IsButtonClicked() || IsKeyPressed(KEY_ESCAPE))
	{
		AppManager::GetInstance()->ChangeScene("res/scenes/main_menu_scene.json");
	}
	else if (m_StartButton.IsButtonClicked() || IsKeyPressed(KEY_ENTER))
	{
		GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"),
			GameManager)->LoadCharacterFromMetadataIndex(m_SelectedElement);
		GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"),
			GameManager)->LoadStartEndTimeFromMetadata();
		AppManager::GetInstance()->ChangeScene("res/scenes/game_scene.json");
	}
}

void Carousel::Animate()
{
	m_Camera.target.x = EaseExpoOut(m_Time, m_OldCamera.target.x, m_TargetCamera.target.x - m_OldCamera.target.x, s_InterpTime);
	//m_Camera.target.y = EaseSineInOut(m_Time, m_OldCamera.target.y, m_TargetCamera.target.y, s_InterpTime);

	if (m_Time > s_InterpTime)
	{
		m_Camera.target = m_TargetCamera.target;
		m_InAnimation = false;
	}

	m_Time += GetFrameTime();
}

void Carousel::DrawElements()
{
	BeginMode2D(m_Camera);
	for (auto& element : m_Elements)
	{
		element->Draw();
	}
	EndMode2D();
}

void Carousel::DrawButtons()
{
	m_StartButton.Draw();
	m_BackButton.Draw();
}

void Carousel::Draw()
{
	HandleInput();

	if (m_InAnimation)
	{
		Animate();
	}

	DrawElements();
	DrawButtons();
}

void Carousel::AdjustCamera(Camera2D& camera, int& selectedElement)
{
	Vector2 winRes = { static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) };
	camera = Camera2D{
		{ winRes.x / 2.f, winRes.y / 2.f },
		{ winRes.x / 2.f * CarouselElement::s_SizePercentage * (selectedElement * 2 + 1) + Carousel::s_Spacing * selectedElement, winRes.y / 2.f * CarouselElement::s_SizePercentage},
		0.f, 1.f
	};
}

void Carousel::AdjustElements(std::vector<std::shared_ptr<CarouselElement>>& m_Elements)
{
	Vector2 winRes = { static_cast<float>(GetScreenHeight()), static_cast<float>(GetScreenWidth()) };
	int i = 0;
	for (auto& element : m_Elements)
	{
		element->m_Position = { element->GetSize().x * i + Carousel::s_Spacing * i, 0 };
		element->m_StatsViewer.m_Position = element->m_Position;
		i++;
	}
}

void Carousel::AdjustButtons(Button& startButton, Button& backButton)
{
	auto startButtonSize = startButton.GetSize();
	startButton.m_Position = Vector2{ GetScreenWidth() / 2.f - startButtonSize.x / 2.f, GetScreenHeight() - startButtonSize.y - 20.f };

	auto backButtonSize = backButton.GetSize();
	backButton.m_Position = Vector2{ 20.f, GetScreenHeight() - startButtonSize.y - 20.f };
}