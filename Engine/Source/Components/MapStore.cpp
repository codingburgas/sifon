#include "./MapStore.hpp"
#include "../Util/file_reader.hpp"

void MapStore::OnCreate()
{
	auto mapLoader = MapReader(Vec2f{ 0.f, 0.f }, { GetScreenWidth() * 1.f, GetScreenHeight() * 1.f }, "name");
	mapLoader.Load(m_FilePath);

	m_CountryTable = mapLoader.GetCountryTable();
}