#include "./GameManager.hpp"
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>

bool IsYearLeap(unsigned& year)
{
	return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

unsigned GetMaxMonth()
{
	return 12;
}

unsigned GetMaxDayOfMonth(unsigned& month, unsigned& year)
{
	if (month == 2)
	{
		if (IsYearLeap(year)) return 29;
		else return 28;
	}
	else if (month <= 7)
	{
		if (month % 2 == 0) return 30;
		else return 31;
	}
	else
	{
		if (month % 2 == 0) return 31;
		else return 30;
	}
}

void GameManager::OnCreate()
{
	
}

void GameManager::OnUpdate()
{
	if (IsKeyPressed(KEY_ESCAPE))
		SetPaused(!GetPaused());
}


void GameManager::AlterMoney(int by)
{
	m_State.m_Resources.m_Money += by;
}

void GameState::Date::SetDay(unsigned day)
{
	m_Day = std::clamp(day, 1u, GetMaxDayOfMonth(m_Month, m_Year));
}

void GameState::Date::SetMonth(unsigned month)
{
	m_Month = std::clamp(month, 1u, 12u);
}

void GameState::Date::SetYear(unsigned year)
{
	m_Year = std::clamp(year, 0u, UINT_MAX);
}

void GameState::Date::AdvanceDays(unsigned days)
{
	m_Day += days;
	while (m_Day > GetMaxDayOfMonth(m_Month, m_Year))
	{
		m_Day -= GetMaxDayOfMonth(m_Month, m_Year);
		AdvanceMonths(1);
	}
}

void GameState::Date::AdvanceMonths(unsigned months)
{
	m_Month += months;
	while (m_Month > 12)
	{
		m_Month -= 12;
		AdvanceYears(1);
	}
}

void GameState::Date::AdvanceYears(unsigned years)
{
	m_Year += years;
}

unsigned GameState::Date::GetMonth()
{
	return m_Month;
}

unsigned GameState::Date::GetYear()
{
	return m_Year;
}

unsigned GameState::Date::GetDay()
{
	return m_Day;
}

float GameManager::GetTodayWeather()
{
	unsigned year = m_State.m_Date.GetYear();
	unsigned month = m_State.m_Date.GetMonth();
	unsigned day = m_State.m_Date.GetDay();

	float monthAndDays = static_cast<float>(month) + static_cast<float>(day) / static_cast<float>(GetMaxDayOfMonth(month, year));
	return abs(sin((monthAndDays / 12) * PI) * 25);
}

void GameManager::LoadCharacterFromMetadataIndex(size_t index)
{
	// get metadata
	auto& metadata = GET_COMPONENT_FROM(
		EntityManager::GetInstance()->GetEntityFromTagName("MappackMetadata"),
		MappackMetadataStore)->GetMetadata();

	if (!(metadata.m_Characters.size() > index))
		throw std::runtime_error("Index is out of bounds of the metadata character array");

	auto character = metadata.m_Characters.at(index);
	m_State.m_CharacterName = character.m_Name;
	for (auto& [type, amount] : character.m_Boosts)
		m_State.m_FactorScales[type] = amount;
}

bool GameManager::LoadSavefile()
{
	using namespace nlohmann;

	if (!FileExists("save.file"))
		return false;

	auto json = json::parse(GetFileContents("save.file"));
	if (!json.is_object())
		return false;

	if (!json["organisations"].is_array())
		return false;

	for (auto& organisation : json["organisations"])
		m_State.m_RegionOrganisations.push_back(organisation.get<std::string>());

	int i = 0;
	for (auto& factorScale : json["factorScales"])
	{
		m_State.m_FactorScales[i] = factorScale;
		if (i > FACTORS_COUNT - 1)
			break;
		i++;
	}

	json["factorsAmount"][Metadata::Factor::CLOTHES].get_to(m_State.m_Resources.m_Clothes);
	json["factorsAmount"][Metadata::Factor::FOOD_WATER].get_to(m_State.m_Resources.m_Food);
	json["factorsAmount"][Metadata::Factor::HOPE].get_to(m_State.m_Community.m_HopePercent);
	json["factorsAmount"][Metadata::Factor::LOYALTY].get_to(m_State.m_Community.m_LoyaltyPercent);
	json["factorsAmount"][Metadata::Factor::MANPOWER].get_to(m_State.m_Resources.m_Manpower);
	json["factorsAmount"][Metadata::Factor::MELEE].get_to(m_State.m_Resources.m_MeleeAmount);
	json["factorsAmount"][Metadata::Factor::MONEY].get_to(m_State.m_Resources.m_Money);
	json["factorsAmount"][Metadata::Factor::MORAL].get_to(m_State.m_Community.m_MoralPercent);

	return true;
}

bool GameManager::SaveSavefile()
{
	return true;
}

void GameManager::SetPaused(bool isPaused)
{
	m_State.m_IsPaused = isPaused;
}

bool GameManager::GetPaused()
{
	return m_State.m_IsPaused;
}

void GameManager::SetCharacterName(std::string name)
{
	m_State.m_CharacterName = name;// TODO: implement
}

void GameManager::SetFactorScales(std::array<float, FACTORS_COUNT> factors)
{
	m_State.m_FactorScales = factors;
}

void GameManager::MakeOrganisationInRegion(std::string region)
{
	m_State.m_Date.AdvanceMonths(2);
	auto& regions = m_State.m_RegionOrganisations;
	if (std::find(regions.begin(), regions.begin(), region) != regions.end())
	{
		regions.push_back(region);
	}
}

int GameManager::GetMoneyAmount()
{
	return m_State.m_Resources.m_Money;
}

int GameManager::GetResourceAmount(GameState::Resource resourceType)
{
	switch (resourceType)
	{
	case GameState::Resource::CLOTHES:
		return m_State.m_Resources.m_Clothes;
	case GameState::Resource::FOOD_WATER:
		return m_State.m_Resources.m_Food;
	case GameState::Resource::MANPOWER:
		return m_State.m_Resources.m_Manpower;
	case GameState::Resource::MELEE:
		return m_State.m_Resources.m_MeleeAmount;
	}

	return -1;
}

float GameManager::GetCommunityPercent(GameState::CommunityResource resourceType)
{
	switch (resourceType)
	{
	case GameState::CommunityResource::HOPE:
		return m_State.m_Community.m_HopePercent;
	case GameState::CommunityResource::LOYALTY:
		return m_State.m_Community.m_LoyaltyPercent;
	case GameState::CommunityResource::MORAL:
		return m_State.m_Community.m_MoralPercent;
	}

	return -1;
}

float GameManager::GetWinProbability()
{
	auto factors = m_State.m_FactorScales;

	float weather = GetTodayWeather();

	float manpower = GetResourceAmount(GameState::Resource::MANPOWER) * factors[2];
	float melee = GetResourceAmount(GameState::Resource::MELEE) * factors[3];
	float food = GetResourceAmount(GameState::Resource::FOOD_WATER) * factors[6];
	float clothes = GetResourceAmount(GameState::Resource::CLOTHES) * factors[7];

	float hope = GetCommunityPercent(GameState::CommunityResource::HOPE) * factors[4];
	float loyalty = GetCommunityPercent(GameState::CommunityResource::LOYALTY) * factors[1];
	float moral = GetCommunityPercent(GameState::CommunityResource::MORAL) * factors[0];

	float weatherPercent = - (12.5f - weather) / 2;
	float manpowerPercent = manpower / 120 + melee / manpower * 100 / 4;
	float necessitiesPercent = ((food + clothes) / (manpower * 2) - 1) * 100;
	float communityPercent = - (0.5f - hope) / 2 - (0.5f - loyalty) / 2 - (0.5f - moral) / 2;
	float randomPercent = rand() % 18 - 9.f;

	float totalPercent = weatherPercent + randomPercent + manpowerPercent + necessitiesPercent;
	
	return totalPercent;
}

void GameManager::MakeRevolution()
{
	// TODO: implement
	// NOTE: use GetWinProbability
}

void GameManager::BuyResources(GameState::Resource resourceType, int amount)
{
	// TODO: implement
	// NOTE: use GetMoneyAmount to compare, AlterMoney
}