#pragma once

#include <raylib.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "Util/json.hpp"
#include "Util/la.hpp"

struct Feature {
    nlohmann::json properties;
    std::vector<Polygon> geometry;
};

using FeatureCollection = std::vector<Feature>;

struct Line
{
    Vec2f p1, p2;
};

class MapReader
{
public:
    MapReader(Vec2f minNorm, Vec2f maxNorm, std::string nameKey) : m_MinNormalised(minNorm), m_MaxNormalised(maxNorm), m_NameKey(nameKey) {};
    ~MapReader() = default;

    void Load(const std::string fPath);

    void Parse(const nlohmann::json json);

    FeatureCollection ParseFeatureCollection(const nlohmann::json json);

    Feature ParseFeature(const nlohmann::json json);

    Polygon ParsePolygon(const nlohmann::json json);

    std::vector<Polygon> ParseMultiPolygon(const nlohmann::json json);

    std::unordered_map<std::string, std::vector<Polygon>>& GetCountryTable()
    {
        return m_CountryTable;
    }

    std::unordered_map<std::string, Vec2f>& GetRegionCenters()
    {
        return m_RegionCenters;
    }

private:
    float mapInt(float x, float inMin, float inMax, float outMin, float outMax)
    {
        return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

    void NormalisePoints();
    void CalculateRegionCenters();

    std::unordered_map<std::string, std::vector<Polygon>> m_CountryTable;
    std::unordered_map<std::string, Vec2f> m_RegionCenters;
    Vec2f m_MinCoord{180.f, 180.f};
    Vec2f m_MaxCoord{-180.f, -180.f};
    Vec2f m_MinNormalised{ 0, 0 };
    Vec2f m_MaxNormalised{ 1280, 720 };
    std::string m_NameKey{};
};