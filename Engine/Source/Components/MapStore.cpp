#include "./MapStore.hpp"
#include "../Util/file_reader.hpp"
#include "../Managers/EntityManager.hpp"
#include "./MappackMetadataStore.hpp"

#include <filesystem>

void MapStore::OnCreate()
{
	auto metadataComponent = GET_COMPONENT_FROM(
		EntityManager::GetInstance()->GetEntityFromTagName("MappackMetadata"),
		MappackMetadataStore);
	auto& metadata = metadataComponent->GetMetadata();
	auto mappackPath = metadataComponent->GetMappackPath();
	m_FilePath = (std::filesystem::path(mappackPath) / "regions.geojson").string();

	auto mapLoader = MapReader(Vec2f{ 0.f, 0.f }, { 1280.f, 720.f }, "name");
	mapLoader.Load(m_FilePath);

	m_CountryTable = mapLoader.GetCountryTable();
	m_RegionCenters = mapLoader.GetRegionCenters();
}