#include "../Managers/EntityManager.hpp"
#include "MapRenderer.hpp"
#include "MapStore.hpp"
#include <reasings.h>
#include <memory>
#include <cmath>
#include "rlgl.h"

// https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
bool IsPointInPolygon(Vec2f point, Polygon polygon)
{
	const auto min = [](float a, float b) { return a < b ? a : b; };
	const auto max = [](float a, float b) { return a < b ? b : a; };
	if (polygon.size() == 0) return false;

	float minX = polygon[0].x;
	float maxX = polygon[0].x;
	float minY = polygon[0].y;
	float maxY = polygon[0].y;

	for (auto it = ++polygon.begin(); it != polygon.end(); it++)
	{
		auto& pt = *it;
		minX = min(pt.x, minX);
		maxX = max(pt.x, maxX);
		minY = min(pt.y, minY);
		maxY = max(pt.y, maxY);
	}

	if (point.x < minX || point.x > maxX || point.y < minY || point.y > maxY)
	{
		return false;
	}

	bool inside = false;
	for (size_t i = 0, j = polygon.size() - 1; i < polygon.size(); j = i++)
	{
		if ((polygon[i].y > point.y) != (polygon[j].y > point.y) &&
			point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)
		{
			inside = !inside;
		}
	}

	return inside;
}

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

void HandleInput(const std::unordered_map<std::string, std::vector<Polygon>>& countryTable, Camera2D& m_Camera,
	std::function<bool(std::string)>& callback, std::unordered_map<std::string, bool>& organisationRegions)
{
	if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
	{
		m_Camera.offset = Vec2f{ m_Camera.offset.x + GetMouseDelta().x, m_Camera.offset.y + GetMouseDelta().y };
	}

	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
	{
		bool breakFlag = false;
		Vec2f mousePosInWorld = GetScreenToWorld2D(GetMousePosition(), m_Camera);
		for (auto& [name, polygons] : countryTable)
		{
			for (auto& polygon : polygons)
			{
				if (IsPointInPolygon(mousePosInWorld, polygon))
				{
					organisationRegions[name] = callback(name);
					breakFlag = true;
					break;
				}
			}

			if (breakFlag) break;
		}
	}
}

void UpdateCamera(Camera2D& m_Camera)
{
	if (GetMouseWheelMove() > 0 && m_Camera.zoom != 2.f)
	{
		m_Camera.zoom = 2.f;
		m_Camera.offset = { m_Camera.offset.x - GetScreenWidth() / 2.f, m_Camera.offset.y - GetScreenHeight() / 2.f };
	}
	else if (GetMouseWheelMove() < 0 && m_Camera.zoom != 1.f)
	{
		m_Camera.zoom = 1.f;
		m_Camera.offset = { m_Camera.offset.x + GetScreenWidth() / 2.f, m_Camera.offset.y + GetScreenHeight() / 2.f };
	}
}

void RenderMap(const std::unordered_map<std::string, std::vector<Polygon>>& countryTable)
{
	for (auto& [name, polygons] : countryTable)
	{
		for (auto& polygon : polygons)
		{
			std::vector<Vector2> points;
			for (auto it = polygon.begin(); it != polygon.end() - 1; it++)
			{
				DrawLineEx(*it, *(it + 1), 3.f, WHITE);
			}
		}
	}
}

void RenderCenters(const std::unordered_map<std::string, Vec2f>& regionCenters, const std::unordered_map<std::string, bool>& regionOrganisations)
{
	for (auto& [name, center] : regionCenters)
	{
		Color color;
		try
		{
			if (regionOrganisations.at(name))
				color = GREEN;
			else color = WHITE;
		}
		catch (const std::out_of_range&)
		{
			color = WHITE;
		}
		DrawText(name.c_str(), static_cast<int>(center.x) - MeasureText(name.c_str(), 14) / 2, static_cast<int>(center.y) - 7, 14, color);
	}
}


void MapRenderer::OnUpdate()
{
	auto& countryTable = m_MapStoreComponent->m_CountryTable;
	auto& centersTable = m_MapStoreComponent->m_RegionCenters;

	auto gameManager = GET_COMPONENT_FROM(EntityManager::GetInstance()->GetEntityFromTagName("GameController"), GameManager);
	if (gameManager->GetPaused() || !m_IsShown) return;

	HandleInput(countryTable, m_Camera, m_OnCountryChosenCallback, m_OrganisationRegions);
	UpdateCamera(m_Camera);

	BeginMode2D(m_Camera);
	RenderMap(countryTable);
	RenderCenters(centersTable, m_OrganisationRegions);
	EndMode2D();
}