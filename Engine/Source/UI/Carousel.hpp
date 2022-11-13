#pragma once

#include "./UIElement.hpp"

#include "./CarouselElement.hpp"
#include "./Button.hpp"

#include <vector>
#include <memory>

class Carousel : public UIElement
{
public:
	// Carousel styling across all instances
	static float s_InterpTime;
	static float s_Spacing;
	static Vector2 s_NavButtonSize;

	Carousel(std::vector<std::shared_ptr<CarouselElement>> elements) : m_Elements(elements), m_StartButton("start", GREEN), m_BackButton("back", RED)
	{
		AdjustCamera(m_Camera, m_SelectedElement);
		AdjustElements(m_Elements);
		AdjustButtons(m_StartButton, m_BackButton);
	}

	virtual Vector2 GetSize() override
	{
		return { static_cast<float>(GetScreenHeight()), static_cast<float>(GetScreenWidth()) };
	}

	virtual void Draw() override;

private:
	// handlers
	void HandleInput();
	void HandleSlidingInput();
	void HandleKeyboardInput();

	// animator function
	void Animate();

	// renderers
	void DrawElements();
	void DrawButtons();
	
	// helper functions
	static void AdjustCamera(Camera2D& camera, int& selectedElement);
	static void AdjustElements(std::vector<std::shared_ptr<CarouselElement>>& m_Elements);
	static void AdjustButtons(Button& startButton, Button& backButton);
	void StartAnimation();
	void StopAnimation();

	Button m_StartButton;
	Button m_BackButton;

	float m_Time = 0.f;
	bool m_InAnimation = false;

	std::vector<std::shared_ptr<CarouselElement>> m_Elements;
	int m_SelectedElement = 0;
	Camera2D m_Camera;

	// used for animation purposes
	// m_Camera will interpolate to m_TargetCamera
	Camera2D m_TargetCamera;
	Camera2D m_OldCamera;
};