#pragma once

#include "../Managers/EntityManager.hpp"
#include "Component.hpp"
#include "MapStore.hpp"
#include "GameManager.hpp"
#include <functional>

class MapRenderer : public Component
{
public:
	DEFINE_COMPONENT(MapRenderer);
	~MapRenderer() = default;

	void OnCreate() override;
	void OnUpdate() override;


	std::string m_MapStoreEntityTag{};
	std::function<bool(std::string)> m_OnCountryChosenCallback = [](std::string) {return false;};

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

private:
	bool m_IsShown = true;

	std::shared_ptr<MapStore> m_MapStoreComponent{};
	std::unordered_map<std::string, Color> m_CountryColors;
	std::unordered_map<std::string, bool> m_OrganisationRegions;
	Camera2D m_Camera{};
};