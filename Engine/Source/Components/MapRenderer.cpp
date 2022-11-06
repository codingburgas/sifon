#include "../Managers/EntityManager.hpp"
#include "MapRenderer.hpp"
#include "MapStore.hpp"
#include <reasings.h>
#include <memory>
#include <cmath>

void MapRenderer::OnCreate()
{
	m_Camera.zoom = 1.f;

	auto mapStoreEnt = EntityManager::GetInstance()->GetEntityFromTagName(m_MapStoreEntityTag);

	if (mapStoreEnt == nullptr)
		throw std::runtime_error("Entity\"" + m_MapStoreEntityTag + "\"doesn't exist or is initialised earlier than this entity");

	auto mapStore = GET_COMPONENT_FROM(mapStoreEnt, MapStore);

	if (mapStore == nullptr)
		throw std::runtime_error("MapStore component doesn't exist in \"" + m_MapStoreEntityTag + "\" entity!");

	m_MapStoreComponent = mapStore;
}

void UpdateCamera(Camera2D& m_Camera)
{
	const float c_Duration = .5f;
	static float c_Time = 0.f;
	static float c_PreviousTargetZoom = 1.f;
	static float c_TargetZoom = 1.f;

	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		m_Camera.offset = Vec2f{ m_Camera.offset.x + GetMouseDelta().x, m_Camera.offset.y + GetMouseDelta().y };
	}

	if (GetMouseWheelMove() != 0 && c_TargetZoom + GetMouseWheelMove() >= 0.f)
	{
		c_Time = 0.f;
		c_PreviousTargetZoom = c_TargetZoom;
		c_TargetZoom += GetMouseWheelMove();
		if (c_TargetZoom <= 0.f)
			c_TargetZoom = 1.f;
		if (c_TargetZoom == 1.f)
			c_PreviousTargetZoom = 1.f;
	}

	m_Camera.zoom = EaseExpoOut(c_Time, c_TargetZoom, c_TargetZoom - c_PreviousTargetZoom, c_Duration);
	if (GetMouseWheelMove() != 0 && !(c_TargetZoom <= 0.f))
	{
		Vec2f screenCenterToWorld{};
		if (GetMouseWheelMove() < 0)
			screenCenterToWorld = GetScreenToWorld2D({ GetMouseX() * 1.f, GetMouseY() * 1.f }, m_Camera);
		else screenCenterToWorld = GetScreenToWorld2D({ GetMouseX() * 1.f, GetMouseY() * 1.f }, m_Camera);
		m_Camera.target = screenCenterToWorld;
		m_Camera.offset = screenCenterToWorld;
	}

	if (c_Time < c_Duration)
		c_Time += GetFrameTime();
}

void Render(const std::unordered_map<std::string, std::vector<Polygon>>& countryTable, const Camera2D& m_Camera)
{
	for (auto& [name, polygons] : countryTable)
	{
		for (auto& polygon : polygons)
		{
			for (auto pointIter = polygon.begin(); pointIter != polygon.end() - 1; ++pointIter)
			{
				DrawLineEx(*pointIter, *(pointIter + 1), 1.f, WHITE);
			}
		}
	}
}

void MapRenderer::OnUpdate()
{
	auto& countryTable = m_MapStoreComponent->m_CountryTable;

	UpdateCamera(m_Camera);

	BeginMode2D(m_Camera);
	Render(countryTable, m_Camera);
	EndMode2D();
}