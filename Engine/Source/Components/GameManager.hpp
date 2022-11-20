#pragma once

#include "./Component.hpp"
#include "./MappackMetadataStore.hpp"
#include "./Component.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Util/json.hpp"
#include "../Util/file_reader.hpp"

#include "raylib.h"

#include <array>

struct GameState
{
	std::array<float, FACTORS_COUNT> m_FactorScales{};
	std::string m_CharacterName{};
	int m_CharacterMonthlyIncome = 0;

	std::vector<std::string> m_RegionOrganisations{};

	enum class Resource
	{
		MANPOWER = 2,
		GUNS = 3,
		FOOD_WATER = 6,
		CLOTHES = 7
	};

	enum class CommunityResource
	{
		MORAL = 0,
		LOYALTY = 1,
		HOPE = 4,
	};

	const float m_PercentToWin = 70.f;

	bool m_LastRevolutionWon = false;
	struct {
		int m_GunAmount = 0;
		int m_Manpower = 0;
		int m_Money = 0;
		int m_Food = 0;
		int m_Clothes = 0;
		float m_LoyaltyPercent = 0.f;
		float m_MoralPercent = 0.f;
		float m_HopePercent = 0.f;
	} m_LastRevolutionAlters;

	int m_WonRevolutionsCount = 0;
	int m_LostRevolutionsCount = 0;

	struct {
		int m_GunAmount = 1;
		int m_Manpower = 1;
		int m_Money = 1;
		int m_Food = 1;
		int m_Clothes = 1;
	} m_Resources;

	struct {
		float m_LoyaltyPercent = 0.01f;
		float m_MoralPercent = 0.01f;
		float m_HopePercent = 0.01f;
	} m_Community;

	struct Date {
	public:
		unsigned m_Year;
		unsigned m_Month;
		unsigned m_Day;

		Date(unsigned day, unsigned month, unsigned year)
		{
			SetDay(day);
			SetMonth(month);
			SetYear(year);
		}

		void SetDay(unsigned day);
		void SetYear(unsigned year);
		void SetMonth(unsigned month);

		unsigned GetMonth();
		unsigned GetYear();
		unsigned GetDay();
	} m_Date{1u, 1u, 1u}, m_EndingDate{1u, 1u, 1u};

	bool m_IsPaused = false;
};

class GameManager : public Component
{
	GameState m_State;

	// Helper functions
	// Money
	void AlterMoney(int by);

public:
	enum class WinningStatus
	{
		WON = 1,
		PLAYING = 0,
		LOST = -1
	};

	struct Status
	{
		std::string m_Reason;
		WinningStatus m_Status;
	};

	DEFINE_COMPONENT(GameManager);

	void OnCreate() override;
	void OnUpdate() override;

	Status GetGameStatus();

	void SetCharacterName(std::string name);
	void SetFactorScales(std::array<float, FACTORS_COUNT> factors);
	void LoadCharacterFromMetadataIndex(size_t index);
	void LoadStartEndTimeFromMetadata();
	
	float GetTodayWeather(); /* in celsius degrees */
	
	void AdvanceMonths(unsigned count);
	void AdvanceDays(unsigned count);
	void AdvanceYears(unsigned count);

	void AdvanceDay();
	void AdvanceMonth();

	unsigned GetDay();
	unsigned GetMonth();
	unsigned GetYear();

	// Resources
	int GetMoneyAmount(); /* in currency units */
	int GetResourceAmount(GameState::Resource resourceType); /* in units */
	float GetCommunityPercent(GameState::CommunityResource resourceType); /* in percent */

	int GetWonRevolutions();
	int GetLostRevolutions();
	float GetWinProbability(bool withRandomChance = true); /* in percent */

	// Actions
	void MakeOrganisationInRegion(std::string region);
	void MakeRevolution();
	void BuyResources(GameState::Resource resourceType, int amount);

	const std::vector<std::string>& GetOrganisations();

	void SetPaused(bool isPaused);
	bool GetPaused();

	const GameState& GetState()
	{
		return m_State;
	}

	// NOT IMPLEMENTED
	bool LoadSavefile();
	bool SaveSavefile();
};