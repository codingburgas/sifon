#include "Component.hpp"
#include "../Util/graphics.hpp"

class SpriteComponent : public Component
{
public:
    /**
     * @brief Defines a component of type SpriteComponent
     */
    DEFINE_COMPONENT(SpriteComponent);
    virtual ~SpriteComponent() = default;

    void OnCreate() override;
    void OnUpdate() override;
    void OnDestroy() override;

    /**
     * @brief Gets sprite resolutin
     * 
     * @return Returns a Vec2i with the sprite resolution
     */
    Vec2i GetSpriteRes();

    /**
     * @brief The path to the image of the sprite
     */
    std::string ImagePath;

private:
    /**
     * @brief The texture of the sprite
     */
    std::shared_ptr<CTexture> texture;
};