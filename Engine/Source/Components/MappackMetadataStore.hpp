#pragma once

#include "./Component.hpp"
#include "../Util/file_reader.hpp"
#include "../Util/json.hpp"

#include <vector>
#include <map>
#include <cassert>

#define FACTORS_COUNT 8

struct Metadata
{
	enum Factor
	{
		MORAL = 0,
		LOYALTY = 1,
		MANPOWER = 2,
		GUNS = 3,
		HOPE = 4,
		MONEY = 5,
		FOOD_WATER = 6,
		CLOTHES = 7
	};

	struct Character
	{
		std::string m_Name, m_Description, m_ImagePath;
		std::map<Factor, float> m_BoostPercents;
		int m_MonthlyIncome;
	};

	struct {
		unsigned day, month, year;
	} m_StartDate, m_EndDate;

	std::string m_GeoJsonNameKey;

	std::string m_Name;
	std::vector<Character> m_Characters;
};

// component used to deserialise the metadata.json file
// in each mappack consisting of data for the mappack itself
// and the playable characters with it
class MappackMetadataStore : public Component
{
	Metadata m_Metadata;

public:
	DEFINE_COMPONENT(MappackMetadataStore);
	~MappackMetadataStore() = default;

	void OnCreate() override
	{
		using namespace nlohmann;

		json jsonMeta = json::parse(GetFileContents(m_MetadataPath));
		assert(jsonMeta.is_object() && "Invalid Mappack Metadata JSON File!");

		jsonMeta["name"].get_to<std::string>(m_Metadata.m_Name);
		jsonMeta["geoJsonNameKey"].get_to<std::string>(m_Metadata.m_GeoJsonNameKey);

		json startDate = jsonMeta["startDate"];
		startDate["day"].get_to(m_Metadata.m_StartDate.day);
		startDate["month"].get_to(m_Metadata.m_StartDate.month);
		startDate["year"].get_to(m_Metadata.m_StartDate.year);

		json endDate = jsonMeta["endDate"];
		endDate["day"].get_to(m_Metadata.m_EndDate.day);
		endDate["month"].get_to(m_Metadata.m_EndDate.month);
		endDate["year"].get_to(m_Metadata.m_EndDate.year);

		assert(jsonMeta["characters"].is_array() && "Invalid Mappack Metadata JSON File!");
		for (auto& character : jsonMeta["characters"])
		{
			Metadata::Character sCharacter;
			character["name"].get_to(sCharacter.m_Name);
			character["description"].get_to(sCharacter.m_Description);
			character["imagePath"].get_to(sCharacter.m_ImagePath);
			character["monthlyIncome"].get_to(sCharacter.m_MonthlyIncome);

			int i = 0;
			for (auto& boost : character["boosts"])
			{
				boost.get_to(sCharacter.m_BoostPercents[(Metadata::Factor)i]);
				i++;
				if (i > FACTORS_COUNT - 1)
					break;
			}

			m_Metadata.m_Characters.push_back(sCharacter);
		}
	};

	const Metadata& GetMetadata() { return m_Metadata; }

	std::string GetMappackPath()
	{
		size_t pos = m_MetadataPath.find_last_of("\\/");
		if (pos == std::string::npos)
			return "";
		else return m_MetadataPath.substr(0, pos);
	}

	std::string m_MetadataPath{};
};