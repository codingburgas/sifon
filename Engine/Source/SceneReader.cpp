#include "SceneReader.hpp"
#include "Managers/ECSManager.hpp"

#include "Components/TransformComponent2D.hpp"
#include "Components/SpriteComponent.hpp"
#include "Components/MapStore.hpp"
#include "Components/MapRenderer.hpp"
#include "Components/MainMenu.hpp"
#include "Components/MappackMetadataStore.hpp"
#include "Components/CharacterSelectMenu.hpp"
#include "Components/GameUI.hpp"
#include "Components/GameManager.hpp"

#include "Util/file_reader.hpp"
#include "Util/json.hpp"

void InitSceneFromFile(const std::string fPath)
{
    /**
     * @brief Gets the contents of the file in the fPath param with the GetFileContents() function
     */
    std::string contents = GetFileContents(fPath);
    nlohmann::json sJson = nlohmann::json::parse(contents);

    assert(sJson.is_array() && "Invalid Scene JSON File!");

    printf("[SceneReader] INFO: Initializing scene from file %s\n", fPath.c_str());

    /**
     * @brief Create an entity for every entity in the .json file, and give it its tag name if it doesn't already exist
     */
    for (auto& ent : sJson)
    {
        std::shared_ptr<Entity> entity;
        
        try {
            std::string tag;
            ent["TagName"].get_to(tag);

            if (EntityManager::GetInstance()->TagNameAlreadyExists(tag) && !tag.empty())
            {
                printf("[SceneReader] WARN: Entity with tag \"%s\" already exists, thus the current entity will be ignored!\n", tag.c_str());
                continue;
            }

            entity = ECS::CreateEntity();
            entity->Tag = tag;
        } catch(const nlohmann::json::exception&) {
            printf("[SceneReader] WARN: TagName is missing in entity!\n");
            entity = ECS::CreateEntity();
        }

        try {
            ent["DestroyOnReload"].get_to(entity->DestroyOnReload);
        } catch(const std::exception&) {}

        if (!entity->DestroyOnReload && entity->Tag.empty())
            throw std::runtime_error("Cannot have global (DestroyOnReload = true) entities without tags");

        printf("[SceneReader] INFO: Entity with ID %lld, Tag \"%s\" has the following components:\n",
            entity->GetID(), entity->Tag.c_str());

        /**
         * @brief For every component in the just created entity, create it and bind it to it
         */
        for (auto& [name, component] : ent["Components"].items())
        {
            if (name == "TransformComponent2D")
            {
                auto cmp = ECS::CreateComponent<TransformComponent2D>();

                component["Position"]["x"].get_to(cmp->Position.x);
                component["Position"]["y"].get_to(cmp->Position.y);

                component["Rotation"].get_to(cmp->Rotation);

                component["Scale"]["x"].get_to(cmp->Scale.x);
                component["Scale"]["y"].get_to(cmp->Scale.y);

                entity->AddComponent(cmp);
            }
            else if (name == "SpriteComponent")
            {
                auto cmp = ECS::CreateComponent<SpriteComponent>();

                component["ImagePath"].get_to(cmp->ImagePath);

                entity->AddComponent(cmp);
            }
            else if (name == "MapStore")
            {
                auto cmp = ECS::CreateComponent<MapStore>();

                entity->AddComponent(cmp);
            }
            else if (name == "MainMenu")
            {
                entity->AddComponent(ECS::CreateComponent<MainMenu>());
            }
            else if (name == "MapRenderer")
            {
                auto cmp = ECS::CreateComponent<MapRenderer>();

                component["MapStoreEntityTag"].get_to(cmp->m_MapStoreEntityTag);

                entity->AddComponent(cmp);
            }
            else if (name == "CharacterSelectMenu")
            {
                entity->AddComponent(ECS::CreateComponent<CharacterSelectMenu>());
            }
            else if (name == "MappackMetadataStore")
            {
                auto cmp = ECS::CreateComponent<MappackMetadataStore>();

                component["MetadataPath"].get_to(cmp->m_MetadataPath);

                entity->AddComponent(cmp);
            }
            else if (name == "GameUI")
            {
                auto cmp = ECS::CreateComponent<GameUI>();
                
                entity->AddComponent(cmp);
            }
            else if (name == "GameManager")
            {
                entity->AddComponent(ECS::CreateComponent<GameManager>());
            }
            else continue;

            printf("[SceneReader] INFO:    - %s\n", name.c_str());
        }
    }
}