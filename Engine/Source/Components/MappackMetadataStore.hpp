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
		MELEE = 3,
		HOPE = 4,
		MONEY = 5,
		FOOD_WATER = 6,
		CLOTHES = 7
	};

	struct Character
	{
		std::string m_Name;
		std::string m_Description;
		std::string m_ImagePath;
		std::map<Factor, float> m_Boosts;
	};

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

		assert(jsonMeta["characters"].is_array() && "Invalid Mappack Metadata JSON File!");
		for (auto& character : jsonMeta["characters"])
		{
			Metadata::Character sCharacter;
			character["name"].get_to(sCharacter.m_Name);
			character["description"].get_to(sCharacter.m_Description);
			character["imagePath"].get_to(sCharacter.m_ImagePath);

			int i = 0;
			for (auto& boost : character["boosts"])
			{
				boost.get_to(sCharacter.m_Boosts[(Metadata::Factor)i]);
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