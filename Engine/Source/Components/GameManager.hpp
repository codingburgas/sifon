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
	std::array<float, FACTORS_COUNT> m_FactorScales;
	std::string m_CharacterName;

	std::vector<std::string> m_RegionOrganisations;

	enum class Resource
	{
		MANPOWER = 2,
		MELEE = 3,
		FOOD_WATER = 6,
		CLOTHES = 7
	};

	enum class CommunityResource
	{
		MORAL = 0,
		LOYALTY = 1,
		HOPE = 4,
	};

	struct {
		std::string m_Region;
		bool m_IsActive = false;
	} m_CurrentRevolution;

	const float m_PercentToWin = 70.f;

	bool m_LastRevolutionWon;

	int m_WonRevolutionsCount = 0;
	int m_LostRevolutionsCount = 0;

	struct {
		int m_MeleeAmount = 0;
		int m_Manpower = 0;
		int m_Money = 0;
		int m_Food = 0;
		int m_Clothes = 0;
	} m_Resources;

	struct {
		float m_LoyaltyPercent = 0.f;
		float m_MoralPercent = 0.f;
		float m_HopePercent = 0.f;
	} m_Community;

	struct Date {
	private:
		unsigned m_Year;
		unsigned m_Month;
		unsigned m_Day;
	public:
		Date(unsigned day, unsigned month, unsigned year)
		{
			SetDay(day);
			SetMonth(month);
			SetYear(year);
		}

		void SetDay(unsigned day);
		void SetYear(unsigned year);
		void SetMonth(unsigned month);

		void AdvanceMonths(unsigned count);
		void AdvanceDays(unsigned count);
		void AdvanceYears(unsigned count);

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
	DEFINE_COMPONENT(GameManager);

	void OnCreate() override;
	void OnUpdate() override;

	void SetCharacterName(std::string name);
	void SetFactorScales(std::array<float, FACTORS_COUNT> factors);
	void LoadCharacterFromMetadataIndex(size_t index);

	
	float GetTodayWeather(); /* in celsius degrees */

	// Resources
	int GetMoneyAmount(); /* in currency units */
	int GetResourceAmount(GameState::Resource resourceType); /* in units */
	float GetCommunityPercent(GameState::CommunityResource resourceType); /* in percent */

	float GetWinProbability(); /* in percent */

	// Actions
	void MakeOrganisationInRegion(std::string region);
	void MakeRevolution();
	void BuyResources(GameState::Resource resourceType, int amount);

	void SetPaused(bool isPaused);
	bool GetPaused();

	// NOT IMPLEMENTED
	bool LoadSavefile();
	bool SaveSavefile();
};