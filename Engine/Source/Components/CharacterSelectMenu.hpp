#pragma once

#include "./UIBuilder.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Components/MappackMetadataStore.hpp"
#include "../UI/Carousel.hpp"
#include "../UI/CarouselElement.hpp"

#include <filesystem>

class CharacterSelectMenu : public UIBuilder
{
public:
	CharacterSelectMenu() : UIBuilder() {}

	void Build(std::shared_ptr<UILayer>& uiLayer) override
	{
		const std::string titleFont = "res/fonts/Inter/Inter-ExtraBold.ttf";
		const std::string descFont = "res/fonts/Inter/Inter-Medium.ttf";
		auto metadataComponent = GET_COMPONENT_FROM(
			EntityManager::GetInstance()->GetEntityFromTagName("MappackMetadata"),
			MappackMetadataStore);
		auto metadata = metadataComponent->GetMetadata();
		auto mappackPath = metadataComponent->GetMappackPath();

		std::vector<std::shared_ptr<CarouselElement>> elements;

		for (auto& character : metadata.m_Characters)
		{
			elements.push_back(std::make_shared<CarouselElement>(
				(std::filesystem::path(mappackPath) / character.m_ImagePath).string(),
				titleFont, descFont, character.m_Name, character.m_Description, character.m_BoostPercents));
		}

		auto carousel = std::make_shared<Carousel>(elements);
		uiLayer->PushElement(carousel);
	}

private:

};