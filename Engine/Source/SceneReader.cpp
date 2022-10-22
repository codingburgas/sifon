#include "SceneReader.hpp"
#include "Managers/ECSManager.hpp"

#include <fstream>
#include <cassert>
#include <sstream>

#include "./Components/TransformComponent2D.hpp"
#include "./Components/SpriteComponent.hpp"
#include "./Components/UIComponent.hpp"

#include "Util/json.hpp"

/**
 * @brief Gets the contents of the .json file
 * 
 * @param path The path to the .json file
 * 
 * @return Returns a string with all the components on an entity
 */
std::string GetFileContents(const std::string path)
{
    std::stringstream jsonStream;

    std::ifstream fJson;

    /**
     * @brief Open the file specified in the path param
     */
    fJson.open(path, std::ios::in);

    if (!fJson.is_open())
        throw std::runtime_error("File \"" + path + "\" could not be opened");
    
    /**
     * @brief While there is still information on the file, extract it. When there is no more information, close the file
     */
    std::string jsonLine;
    while (std::getline(fJson, jsonLine))
        jsonStream << jsonLine << '\n';
    
    fJson.close();

    /**
     * @brief Return the stringstream with the information on the file
     */
    return jsonStream.str();
}

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
            else if (name == "UIComponent")
            {
                auto cmp = ECS::CreateComponent<UIComponent>();

                component["Type"].get_to(cmp->type);
                component["Text"].get_to(cmp->text);

                entity->AddComponent(cmp);
            }
            else continue;

            printf("[SceneReader] INFO:    - %s\n", name.c_str());
        }
    }
}