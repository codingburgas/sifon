#pragma once

#include "MapReader.hpp"
#include "Util/file_reader.hpp"
#include <algorithm>

void MapReader::Load(const std::string fPath)
{
    nlohmann::json json{};
    {
        std::string contents = GetFileContents(fPath);
        json = nlohmann::json::parse(contents);
    }

    assert(json.is_object() && "Passed JSON File isn't an object!");
    Parse(json);
    NormalisePoints();
    CalculateRegionCenters();
}

void MapReader::Parse(const nlohmann::json json)
{
    auto typeName = json["type"].get<std::string>();
    if (typeName == "FeatureCollection")
    {
        auto result = ParseFeatureCollection(json);
        for (auto& feature : result)
        {
            assert(feature.properties[m_NameKey].is_string() && "The key \"name\" is required as a member in the properties of a Feature");
            auto featureName = feature.properties[m_NameKey].get<std::string>();

            std::cout << "INFO: " << featureName << std::endl;

            m_CountryTable[featureName] = feature.geometry;
        }
    }
    else
        printf("[MapLoader] WARN: %s type not yet implemented\n", typeName.c_str());
}

FeatureCollection MapReader::ParseFeatureCollection(const nlohmann::json json)
{
    // create an array to store polygons
    FeatureCollection collection{};
    //

    auto& featuresJson = json["features"];
    assert(featuresJson.is_array() && "Passed FeatureCollection's \"features\" key isn't an array!");

    for (auto& feature : featuresJson)
    {
        auto featureType = feature["type"].get<std::string>();
        if (featureType == "Feature")
        {
            collection.push_back(ParseFeature(feature));
        }
        else
            printf("[MapLoader] WARN: %s type not yet implemented\n", featureType.c_str());
    }

    // return stored polygons
    return collection;
    //
}

Feature MapReader::ParseFeature(const nlohmann::json json)
{
    auto geometryType = json["geometry"]["type"].get<std::string>();
    auto properties = json["properties"];
    auto& coordinates = json["geometry"]["coordinates"];

    if (geometryType == "Polygon")
        return Feature{ properties, std::vector<Polygon>{ParsePolygon(coordinates)} };
    else if (geometryType == "MultiPolygon")
    {
        auto multiPolygon = ParseMultiPolygon(coordinates);

        return Feature{ properties, multiPolygon };
    }
    else printf("[MapLoader] WARN: %s type not yet implemented\n", geometryType.c_str());

    return Feature{};
}

Polygon MapReader::ParsePolygon(const nlohmann::json json)
{
    Polygon polygon;

    for (auto& coordCollection : json)
    {
        for (auto& point : coordCollection)
        {
            polygon.push_back(Vec2f{
                point[0], point[1]
                });
        }
    }

    return polygon;
}

std::vector<Polygon> MapReader::ParseMultiPolygon(const nlohmann::json json)
{
    std::vector<Polygon> polygons;

    for (auto& polygon : json)
    {
        polygons.push_back(ParsePolygon(polygon));
    }

    return polygons;
}

void MapReader::CalculateRegionCenters()
{
    for (auto& [name, polygons] : m_CountryTable)
    {
        float xSum = 0.f;
        float ySum = 0.f;
        size_t size = 0;

        for (auto& polygon : polygons)
        {
            size += polygon.size();
            for (auto& point : polygon)
            {
                xSum += point.x;
                ySum += point.y;
            }
        }

        m_RegionCenters[name] = Vec2f{xSum / size, ySum / size};
    }
}

void MapReader::NormalisePoints()
{
    {
        auto& firstPoint = (*m_CountryTable.begin()).second[0][0];
        m_MinCoord = firstPoint;
        m_MaxCoord = firstPoint;
    }

    // finding min and max point in currently parsed map
    {
        std::vector<float> xComponents, yComponents;

        for (auto& [name, polygons] : m_CountryTable)
        {
            for (auto& polygon : polygons)
            {
                for (auto& point : polygon)
                {
                    xComponents.push_back(point.x);
                    yComponents.push_back(point.y);
                }
            }
        }

        m_MinCoord.x = *std::min_element(xComponents.begin(), xComponents.end());
        m_MinCoord.y = *std::min_element(yComponents.begin(), yComponents.end());

        m_MaxCoord.x = *std::max_element(xComponents.begin(), xComponents.end());
        m_MaxCoord.y = *std::max_element(yComponents.begin(), yComponents.end());
    }

    auto mapResolution = Vec2f{ m_MaxCoord.x - m_MinCoord.x, m_MaxCoord.y - m_MinCoord.y };
    auto ratio = mapResolution.x / mapResolution.y;

    // remapping the coordinates to the max and min points (normalising)
    for (auto& [name, polygons] : m_CountryTable)
    {
        for (auto& polygon : polygons)
        {
            for (auto& point : polygon)
            {
                point.x = mapInt(point.x, m_MinCoord.x, m_MaxCoord.x, m_MinNormalised.x, m_MaxNormalised.x);
                point.y = mapInt(point.y, m_MaxCoord.y, m_MinCoord.y, m_MinNormalised.y, m_MaxNormalised.y);
            }
        }
    }
}