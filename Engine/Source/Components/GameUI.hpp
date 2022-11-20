#pragma once

#include "./UIBuilder.hpp"
#include "../Components/GameManager.hpp"
#include "../Components/Component.hpp"
#include "../Components/MapRenderer.hpp"
#include "../Managers/EntityManager.hpp"
#include "../UI/ActionsMenu.hpp"
#include "../UI/PauseMenu.hpp"
#include "../UI/TimeMenu.hpp"
#include "../UI/ResourceStatus.hpp"
#include "../UI/LostWonView.hpp"
#include "../UI/EventSummary.hpp"

#include <functional>
#include <map>
#include <algorithm>

class GameUILabel : public UIElement
{
private:
	float m_FontSize;
	std::string m_FontPath;

	Font m_Font;

	bool m_IsShown = false;

public:
	float m_BottomPadding;
	std::string m_Text;
	Color m_Color = WHITE;
	GameUILabel(std::string fontPath, float fontSize, std::string text, float bottomPadding)
		: m_Font(LoadFontEx(m_FontPath.c_str(), static_cast<float>(m_FontSize), nullptr, 0)),
		  m_Text(text), m_FontSize(fontSize), m_FontPath(fontPath), m_BottomPadding(bottomPadding) {}
	~GameUILabel() { UnloadFont(m_Font); }

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

	void Draw() override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
		
		auto textWidth = MeasureTextEx(m_Font, m_Text.c_str(), m_FontSize, 1.f).x;
		if (m_IsShown && !gameManager->GetPaused())
		{
			DrawTextEx(m_Font, m_Text.c_str(),
				{ static_cast<float>(GetScreenWidth()) / 2.f - textWidth / 2.f, static_cast<float>(GetScreenHeight()) - m_BottomPadding - m_FontSize },
				m_FontSize, 1.f, m_Color);
		}
	}

	Vector2 GetSize() { return {}; }
};

class GameUI : public UIBuilder
{
public:
	void Build(std::shared_ptr<UILayer>& uiLayer) override
	{
		auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);

		std::shared_ptr<GameUILabel> rLabel = std::make_shared<GameUILabel>("res/fonts/Inter/Inter-ExtraBold.ttf", 40.f, "", 190.f);
		rLabel->Hide();
		std::shared_ptr<GameUILabel> oLabel = std::make_shared<GameUILabel>("res/fonts/Inter/Inter-ExtraBold.ttf", 40.f, "Choose a Region!", 120.f);
		oLabel->Hide();
		auto eventSummary = std::make_shared<EventSummary>();

		std::shared_ptr<int> multiplier = std::make_shared<int>(100);
		std::shared_ptr<bool> isChoosingRegion = std::make_shared<bool>(false);
		std::map<std::string, std::function<void()>> resourcesButtonData{
			{"G", [multiplier, gameManager]() { gameManager->BuyResources(GameState::Resource::GUNS, *multiplier); }},
			{"FW", [multiplier, gameManager]() { gameManager->BuyResources(GameState::Resource::FOOD_WATER, *multiplier); }},
			{"C", [multiplier, gameManager]() { gameManager->BuyResources(GameState::Resource::CLOTHES, *multiplier); }},
			{"+", [multiplier, rLabel]() { (*multiplier)++; rLabel->m_Text = std::format("Count: {}", *multiplier); }},
			{"-", [multiplier, rLabel]() { *multiplier = std::clamp(*multiplier - 1, 1, *multiplier); rLabel->m_Text = std::format("Count: {}", *multiplier); }}
		};

		std::shared_ptr<ActionsMenu> rMenu = std::make_shared<ActionsMenu>(resourcesButtonData, 120.f);
		rMenu->Hide();

		std::map<std::string, std::function<void()>> actionsButtonData{
			{"Revolution", [rMenu, rLabel, gameManager, eventSummary]() {
				rMenu->Hide();
		        rLabel->Hide();
				gameManager->MakeRevolution();
				eventSummary->Show();
		    }},
			{"Organisation", [rMenu, rLabel, oLabel, isChoosingRegion, gameManager]() {
				auto mapRenderer = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("Game"), MapRenderer);
				rMenu->Hide();
			    rLabel->Hide();
				if (!(*isChoosingRegion))
				{
					*isChoosingRegion = true;
					oLabel->Show();
					mapRenderer->m_OnCountryChosenCallback = [gameManager](std::string region) {
						gameManager->MakeOrganisationInRegion(region);
						return true;
					};
				}
				else
				{
					*isChoosingRegion = false;
					oLabel->Hide();
					mapRenderer->m_OnCountryChosenCallback = [](std::string) { return false; };
				}
			}},
			{"Resources", [rMenu, rLabel, oLabel, isChoosingRegion, multiplier]() {
				auto mapRenderer = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("Game"), MapRenderer);
				*isChoosingRegion = false;
				oLabel->Hide();
				mapRenderer->m_OnCountryChosenCallback = [](std::string) { return false; };

				rMenu->ToggleShow();
				rLabel->ToggleShow();
				rLabel->m_Text = std::format("Count: {}", *multiplier);
			}}
		};

		std::shared_ptr<ActionsMenu> aMenu = std::make_shared<ActionsMenu>(actionsButtonData, 40.f);
		std::shared_ptr<PauseMenu> pMenu = std::make_shared<PauseMenu>();
		std::shared_ptr<TimeMenu> tMenu = std::make_shared<TimeMenu>();
		std::shared_ptr<ResourceStatus> rStatus = std::make_shared<ResourceStatus>();

		std::function<void(bool)> hideAll = [aMenu, rMenu, tMenu, rStatus, rLabel, oLabel, pMenu, eventSummary](bool with) {
			if (with)
			{
				auto mapRenderer = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("Game"), MapRenderer);
				mapRenderer->Hide();
			}
			aMenu->Hide();
			rMenu->Hide();
			tMenu->Hide();
			rStatus->Hide();
			rLabel->Hide();
			oLabel->Hide();
			pMenu->Hide();
			eventSummary->Hide();
		};

		std::function<void(bool)> showAll = [aMenu, rMenu, tMenu, rStatus, rLabel, oLabel, pMenu, eventSummary](bool with) {
			if (with)
			{
				auto mapRenderer = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("Game"), MapRenderer);
				mapRenderer->Show();
			}
			aMenu->Show();
			rMenu->Show();
			tMenu->Show();
			rStatus->Show();
			rLabel->Show();
			oLabel->Show();
			pMenu->Show();
			eventSummary->Show();
		};

		uiLayer->PushElement(aMenu);
		uiLayer->PushElement(rMenu);
		uiLayer->PushElement(tMenu);
		uiLayer->PushElement(rStatus);
		uiLayer->PushElement(rLabel);
		uiLayer->PushElement(oLabel);
		uiLayer->PushElement(pMenu);
		uiLayer->PushElement(eventSummary);
		uiLayer->PushElement(std::make_shared<LostWonView>([hideAll]() { hideAll(true); }));
	}
};