#pragma once

#include "Component.hpp"
#include <unordered_map>
#include "../MapReader.hpp"

class MapStore : public Component
{
public:
	DEFINE_COMPONENT(MapStore);
	~MapStore() = default;

	void OnCreate() override;

	std::string m_FilePath{};
	std::unordered_map<std::string, std::vector<Polygon>> m_CountryTable{};
	std::unordered_map<std::string, Vec2f> m_RegionCenters{};
};