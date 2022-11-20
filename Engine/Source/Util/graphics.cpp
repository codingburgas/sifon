#include "graphics.hpp"
#include "../config.h"

// lol, remember that :: is scope resolution operator

bool Graphics::InitWindow(Vec2f winDimensions, const char* title)
{
    ::InitWindow(static_cast<int>(winDimensions.x), static_cast<int>(winDimensions.y), title);
    return true;
}

bool Graphics::CloseWindow()
{
    ::CloseWindow();
    return true;
}

Vec2i Graphics::GetWindowDimensions()
{
    return Vec2i {
        GetScreenWidth(),
        GetScreenHeight()
    };
}

void Graphics::BeginDrawing()
{
    if (WindowShouldClose())
        AppManager::GetInstance()->Quit();

    ::BeginDrawing();
    ::ClearBackground(BLACK);
}

void Graphics::EndDrawing()
{
    ::EndDrawing();
}

void Graphics::SetWindowPosition(Vec2f pos)
{
    ::SetWindowPosition(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

// TODO!
void CTexture::LoadEmpty(Vec2i res)
{
    Unload();

    renderTexture = LoadRenderTexture(res.x, res.y);
    verticallyMirroredTexture = LoadRenderTexture(res.x, res.y);

    if (renderTexture.texture.id == 0 || verticallyMirroredTexture.texture.id == 0)
        throw std::runtime_error("Frame buffer (render texture) could not be created");
}

void CTexture::LoadFromFile(const char *path)
{
    Unload();

    texture = LoadTexture(path);

    if (texture.id == 0)
    {
        printf("FATAL: Texture could not be loaded [%s]\n", path);
        throw std::runtime_error("Texture \"" + std::string{path} + "\" could not be loaded!");
    }
}

void CTexture::Unload()
{
    UnloadTexture(texture);
    UnloadRenderTexture(renderTexture);
    UnloadRenderTexture(verticallyMirroredTexture);
}

void CTexture::Draw(Recti Source, Rectf Destination, double Rotation, Vec2f Origin)
{
    if (renderTexture.id != 0)
        DrawTexturePro(renderTexture.texture,
            {
                -static_cast<float>(renderTexture.texture.width + Source.x),
                static_cast<float>(renderTexture.texture.height - Source.y),
                static_cast<float>(renderTexture.texture.width),
                -static_cast<float>(renderTexture.texture.height)
            },
            Destination, Origin, static_cast<float>(Rotation), WHITE);
    if (texture.id != 0)
        DrawTexturePro(texture, Source, Destination, Origin, static_cast<float>(Rotation), Color { 255, 255, 255, 255 });
}

void CTexture::Draw(Vec2f Position, Vec2f Scale, double Rotation, Vec2f Origin)
{
    if (renderTexture.id != 0)
        DrawTexturePro(renderTexture.texture,
            {
                -static_cast<float>(renderTexture.texture.width),
                static_cast<float>(renderTexture.texture.height),
                static_cast<float>(renderTexture.texture.width),
                -static_cast<float>(renderTexture.texture.height)
            },
            {
                0, 0,
                static_cast<float>(renderTexture.texture.width) * Scale.x,
                static_cast<float>(renderTexture.texture.height) * Scale.y
            },
            Origin, static_cast<float>(Rotation), WHITE);
    if (texture.id != 0)
        DrawTexturePro(texture,
            { 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height) },
            { Position.x, Position.y, Scale.x * static_cast<float>(texture.width), Scale.y * static_cast<float>(texture.height) },
            Origin, static_cast<float>(Rotation), WHITE);
}

Vec2i CTexture::GetSize()
{
    if (renderTexture.id != 0)
        return { renderTexture.texture.width, renderTexture.texture.height };
    if (texture.id != 0)
        return { texture.width, texture.height };
    return {};
}

CTexture::~CTexture()
{
    this->Unload();
}

void CTexture::BeginDrawingTo()
{
    if (renderTexture.id == 0) return;
    BeginTextureMode(renderTexture);
}

void CTexture::EndDrawingTo()
{
    if (renderTexture.id == 0) return;
    EndTextureMode();

    BeginTextureMode(verticallyMirroredTexture);
        DrawTexture(renderTexture.texture, 0, 0, WHITE);
    EndTextureMode();
}

Texture& CTexture::GetTexture()
{
    return texture;
}

RenderTexture2D& CTexture::GetRenderTexture()
{
    return verticallyMirroredTexture;
}

void CShader::Load(std::string vertexShader, std::string fragmentShader)
{
    const char* vShader = (vertexShader == "" ? nullptr : vertexShader.c_str());
    const char* fShader = (fragmentShader == "" ? nullptr : fragmentShader.c_str());

    m_Shader = LoadShader(vShader, fShader);
}

void CShader::Unload()
{
    UnloadShader(m_Shader);
}

void CShader::SetShaderValue(std::string uniformName, std::any value, int type)
{
    if (m_Shader.id == 0) return;

    if (m_ShaderLocs.find(uniformName) == m_ShaderLocs.end())
    {
        m_ShaderLocs[uniformName] = ::GetShaderLocation(m_Shader, uniformName.c_str());
    }

    switch (type)
    {
    case SHADER_UNIFORM_FLOAT:
    {
        float data = std::any_cast<float>(value);
        ::SetShaderValue(m_Shader, m_ShaderLocs.at(uniformName), &data, type);
        break;
    }
    case SHADER_UNIFORM_VEC2:
    {
        Vector2 data = std::any_cast<Vector2>(value);
        ::SetShaderValue(m_Shader, m_ShaderLocs.at(uniformName), &data, type);
        break;
    }
    case SHADER_UNIFORM_VEC3:
    {
        Vector3 data = std::any_cast<Vector3>(value);
        ::SetShaderValue(m_Shader, m_ShaderLocs.at(uniformName), &data, type);
        break;
    }
    case SHADER_UNIFORM_VEC4:
    {
        Vector4 data = std::any_cast<Vector4>(value);
        ::SetShaderValue(m_Shader, m_ShaderLocs.at(uniformName), &data, type);
        break;
    }
    case SHADER_UNIFORM_INT:
    {
        int data = std::any_cast<int>(value);
        ::SetShaderValue(m_Shader, m_ShaderLocs.at(uniformName), &data, type);
    }
    }
}

Shader& CShader::GetShader()
{
    return m_Shader;
}

bool Input::IsKeyDown(int key)
{
    return ::IsKeyDown(key);
}

bool Input::IsKeyUp(int key)
{
    return ::IsKeyUp(key);
}

Vec2i Input::GetMousePos()
{
    Vec2f pos = GetMousePosition();
    return { static_cast<int>(pos.x), static_cast<int>(pos.y) };
}

Vec2i Input::GetMouseDelta()
{
    Vec2f delta = ::GetMouseDelta();
    return { static_cast<int>(delta.x), static_cast<int>(delta.y) };
}

void Input::SetMousePos(Vec2i pos)
{
    ::SetMousePosition(pos.x, pos.y);
}

bool Input::IsMouseButtonDown(int button)
{
    return ::IsMouseButtonDown(button);
}

bool Input::IsMouseButtonUp(int button)
{
    return ::IsMouseButtonUp(button);
}