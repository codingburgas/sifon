#pragma once

#include "../Managers/EntityManager.hpp"
#include "Component.hpp"
#include "MapStore.hpp"
#include "GameManager.hpp"

class MapRenderer : public Component
{
public:
	DEFINE_COMPONENT(MapRenderer);
	~MapRenderer() = default;

	void OnCreate() override;
	void OnUpdate() override;

	std::string m_MapStoreEntityTag{};

private:
	std::shared_ptr<MapStore> m_MapStoreComponent{};
	Camera2D m_Camera{};
};