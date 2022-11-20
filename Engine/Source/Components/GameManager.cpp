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

void GameManager::AdvanceDays(unsigned days)
{
	auto& m_Day = m_State.m_Date.m_Day;
	auto& m_Month = m_State.m_Date.m_Month;
	auto& m_Year = m_State.m_Date.m_Year;
	m_Day += days;
	while (m_Day > GetMaxDayOfMonth(m_Month, m_Year))
	{
		m_Day -= GetMaxDayOfMonth(m_Month, m_Year);
		AdvanceMonths(1);
	}
}

void GameManager::AdvanceMonths(unsigned months)
{
	auto& m_Month = m_State.m_Date.m_Month;
	AlterMoney(m_State.m_CharacterMonthlyIncome * months);
	m_Month += months;
	while (m_Month > 12)
	{
		m_Month -= 12;
		AdvanceYears(1);
	}
	AdvanceDays(0);
}

void GameManager::AdvanceYears(unsigned years)
{
	auto& m_Year = m_State.m_Date.m_Year;
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

void ResetState(GameState& m_State)
{
	m_State.m_FactorScales = {};
	m_State.m_CharacterName = {};
	m_State.m_CharacterMonthlyIncome = {};
	m_State.m_RegionOrganisations = {};
	m_State.m_LastRevolutionWon = false;
	m_State.m_WonRevolutionsCount = {};
	m_State.m_LostRevolutionsCount = {};
	m_State.m_Resources = {};
	m_State.m_Community = {};
	m_State.m_Date = { 1u, 1u, 1u };
	m_State.m_EndingDate = { 1u, 1u, 1u };
	m_State.m_IsPaused = false;
}

void GameManager::LoadCharacterFromMetadataIndex(size_t index)
{
	ResetState(m_State);

	// get metadata
	auto& metadata = GET_COMPONENT_FROM(
		EntityManager::GetInstance()->GetEntityFromTagName("MappackMetadata"),
		MappackMetadataStore)->GetMetadata();

	if (!(metadata.m_Characters.size() > index))
		throw std::runtime_error("Index is out of bounds of the metadata character array");

	auto& character = metadata.m_Characters.at(index);
	m_State.m_CharacterName = character.m_Name;
	m_State.m_CharacterMonthlyIncome = character.m_MonthlyIncome;
	for (auto& [type, amount] : character.m_BoostPercents)
		m_State.m_FactorScales[type] = amount;

	m_State.m_Community.m_HopePercent = static_cast<float>(10 + rand() % 20) / 100.f;
	m_State.m_Community.m_LoyaltyPercent = static_cast<float>(20 + rand() % 40) / 100.f;
	m_State.m_Community.m_MoralPercent = static_cast<float>(20 + rand() % 40) / 100.f;
	AlterMoney(500 + rand() % 1000);
}

void GameManager::LoadStartEndTimeFromMetadata()
{
	auto& metadata = GET_COMPONENT_FROM(
		EntityManager::GetInstance()->GetEntityFromTagName("MappackMetadata"),
		MappackMetadataStore)->GetMetadata();

	{
		auto& [day, month, year] = metadata.m_StartDate;
		m_State.m_Date = GameState::Date(day, month, year);
	}

	{
		auto& [day, month, year] = metadata.m_EndDate;
		m_State.m_EndingDate = GameState::Date(day, month, year);
	}
}

void GameManager::AdvanceDay()
{
	AdvanceDays(1);
}

void GameManager::AdvanceMonth()
{
	AdvanceMonths(1);
}

unsigned GameManager::GetDay()
{
	return m_State.m_Date.GetDay();
}

unsigned GameManager::GetMonth()
{
	return m_State.m_Date.GetMonth();
}

unsigned GameManager::GetYear()
{
	return m_State.m_Date.GetYear();
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
	json["factorsAmount"][Metadata::Factor::GUNS].get_to(m_State.m_Resources.m_GunAmount);
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
	m_State.m_CharacterName = name;
}

void GameManager::SetFactorScales(std::array<float, FACTORS_COUNT> factors)
{
	m_State.m_FactorScales = factors;
}

void GameManager::MakeOrganisationInRegion(std::string region)
{
	auto& regions = m_State.m_RegionOrganisations;
	if (std::find(regions.begin(), regions.end(), region) == regions.end())
	{
		AdvanceMonths(2);
		m_State.m_Community.m_HopePercent += static_cast<float>(5 + rand() % 5) / 100.f;
		m_State.m_Resources.m_Manpower += 1000;
		regions.push_back(region);
	}
}

const std::vector<std::string>& GameManager::GetOrganisations()
{
	return m_State.m_RegionOrganisations;
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
	case GameState::Resource::GUNS:
		return m_State.m_Resources.m_GunAmount;
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

float GameManager::GetWinProbability(bool withRandomChance)
{
	const auto& factors = m_State.m_FactorScales;

	float weather = GetTodayWeather();

	float manpower = GetResourceAmount(GameState::Resource::MANPOWER) * factors[2];
	float guns = GetResourceAmount(GameState::Resource::GUNS) * factors[3];
	float food = GetResourceAmount(GameState::Resource::FOOD_WATER) * factors[6];
	float clothes = GetResourceAmount(GameState::Resource::CLOTHES) * factors[7];

	float hope = GetCommunityPercent(GameState::CommunityResource::HOPE) * factors[4];
	float loyalty = GetCommunityPercent(GameState::CommunityResource::LOYALTY) * factors[1];
	float moral = GetCommunityPercent(GameState::CommunityResource::MORAL) * factors[0];

	float weatherPercent = -(12.5f - weather) / 2;
	float manpowerPercent = manpower / 125 + guns / manpower * 100 / 5;
	float necessitiesPercent = std::clamp(food + clothes, 0.f, manpower * 2 + 5000) / (manpower * 2) * 12 - 10;
	float communityPercent = -(1.5f - hope - loyalty - moral) * 10;
	float randomPercent;

	if (withRandomChance)
		randomPercent = rand() % 18 - 9.f;
	else randomPercent = 0.f;

	float totalPercent = weatherPercent + randomPercent + manpowerPercent + necessitiesPercent;
	
	return totalPercent;
}

int GameManager::GetWonRevolutions()
{
	return m_State.m_WonRevolutionsCount;
}
int GameManager::GetLostRevolutions()
{
	return m_State.m_LostRevolutionsCount;
}

void GameManager::MakeRevolution()
{
	AdvanceMonths(1);

	int money = GetMoneyAmount();
	int manpower = GetResourceAmount(GameState::Resource::MANPOWER);
	int guns = GetResourceAmount(GameState::Resource::GUNS);
	int food = GetResourceAmount(GameState::Resource::FOOD_WATER);
	int clothes = GetResourceAmount(GameState::Resource::CLOTHES);
	float hope = GetCommunityPercent(GameState::CommunityResource::HOPE);
	float loyalty = GetCommunityPercent(GameState::CommunityResource::LOYALTY);
	float moral = GetCommunityPercent(GameState::CommunityResource::MORAL);

	int newMoney = rand() % 20000;
	float newHope, newLoyalty, newMoral;

	if (GetWinProbability() >= m_State.m_PercentToWin)
	{
		m_State.m_WonRevolutionsCount++;
		m_State.m_LastRevolutionWon = true;

		newHope = hope + rand() % 40 / 100.f;
		newLoyalty = loyalty + rand() % 40 / 100.f;
		newMoral = moral + rand() % 40 / 100.f;

		AlterMoney(newMoney);
		m_State.m_Community.m_HopePercent = std::clamp(newHope, 0.01f, 1.f);
		m_State.m_Community.m_LoyaltyPercent = std::clamp(newLoyalty, 0.01f, 1.f);
		m_State.m_Community.m_MoralPercent = std::clamp(newMoral, 0.01f, 1.f);
	}
	else
	{
		m_State.m_LostRevolutionsCount++;
		m_State.m_LastRevolutionWon = false;

		newHope = hope - rand() % 40 / 100.f;
		newLoyalty = loyalty - rand() % 40 / 100.f;
		newMoral = moral - rand() % 40 / 100.f;

		AlterMoney(-newMoney);
		m_State.m_Community.m_HopePercent = std::clamp(newHope, 0.01f, 1.f);
		m_State.m_Community.m_LoyaltyPercent = std::clamp(newLoyalty, 0.01f, 1.f);
		m_State.m_Community.m_MoralPercent = std::clamp(newMoral, 0.01f, 1.f);
	}

	int newManpower = manpower - rand() % 2000;
	int newGuns = guns - rand() % 2000;
	int newFood = food - rand() % 2000;
	int newClothes = clothes - rand() % 2000;

	m_State.m_Resources.m_Manpower = std::clamp(newManpower, 1, INT_MAX);
	m_State.m_Resources.m_GunAmount = std::clamp(newGuns, 1, INT_MAX);
	m_State.m_Resources.m_Food = std::clamp(newFood, 1, INT_MAX);
	m_State.m_Resources.m_Clothes = std::clamp(newClothes, 1, INT_MAX);

	m_State.m_LastRevolutionAlters.m_LoyaltyPercent = newLoyalty - loyalty;
	m_State.m_LastRevolutionAlters.m_HopePercent = newHope - hope;
	m_State.m_LastRevolutionAlters.m_MoralPercent = newMoral - moral;

	m_State.m_LastRevolutionAlters.m_GunAmount = newGuns - guns;
	m_State.m_LastRevolutionAlters.m_Food = newFood - food;
	m_State.m_LastRevolutionAlters.m_Manpower = newManpower - manpower;
	m_State.m_LastRevolutionAlters.m_Clothes = newClothes - clothes;
}

void GameManager::BuyResources(GameState::Resource resourceType, int amount)
{
	const int gunCost = 10;
	const int foodCost = 3;
	const int clothesCost = 4;

	int resourceCost = amount;

	switch (resourceType)
	{
	case GameState::Resource::GUNS:
		resourceCost *= gunCost;

		if (GetMoneyAmount() - resourceCost < 0) return;

		m_State.m_Resources.m_GunAmount += amount;
		break;
	case GameState::Resource::FOOD_WATER:
		resourceCost *= foodCost;

		if (GetMoneyAmount() - resourceCost < 0) return;

		m_State.m_Resources.m_Food += amount;
		break;
	case GameState::Resource::CLOTHES:
		resourceCost *= clothesCost;

		if (GetMoneyAmount() - resourceCost < 0) return;

		m_State.m_Resources.m_Clothes += amount;
		break;
	}

	AlterMoney(-resourceCost);
}