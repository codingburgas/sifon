#include "SpriteComponent.hpp"
#include "TransformComponent2D.hpp"
#include "../Util/graphics.hpp"

void SpriteComponent::OnCreate()
{
    /**
     * @brief Creates object and loads the image for the sprite when it's created
     */
    texture = std::make_shared<CTexture>();
    texture->LoadFromFile(ImagePath.c_str());
}

void SpriteComponent::OnDestroy() {}

void SpriteComponent::OnUpdate()
{
    /**
     * @brief If the texture isn't expired, get its size and draw it
     */
    std::shared_ptr<TransformComponent2D> transform = GET_COMPONENT(TransformComponent2D);

    auto texRes = texture->GetSize();

    texture->Draw(transform->Position, transform->Scale,
        transform->Rotation, Vec2f {});
}

Vec2i SpriteComponent::GetSpriteRes()
{
    /**
     * @brief Get the size of the texture and return it
     */
    return texture->GetSize();
}