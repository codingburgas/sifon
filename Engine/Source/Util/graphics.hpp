#pragma once
#include <cstdio>
#include <cassert>
#include <memory>
#include <any>
#include <unordered_map>
#include "../Managers/AppManager.hpp"
#include "la.hpp"
#include "raylib.h"

// The class Texture's purpose is to encapsulate raylib's
// primitive struct type and to simplify its usage in an
// object oriented manner
class CTexture
{
public:
    CTexture() = default;

    virtual ~CTexture();

    void LoadEmpty(Vec2i res);
    void LoadFromFile(const char *path);
    void Unload();

    void Draw(Recti Source, Rectf Destination, double Rotation, Vec2f Origin);
    void Draw(Vec2f Position, Vec2f Scale, double Rotation, Vec2f Origin);

    void BeginDrawingTo();
    void EndDrawingTo();

    Vec2i GetSize();
    Texture& GetTexture();
    RenderTexture& GetRenderTexture();

private:
    Texture texture {};
    RenderTexture renderTexture {};
    RenderTexture verticallyMirroredTexture {};
};

// The class shader's purpose is to encapsulate
// raylib's primitive Shader struct type and to
// simplify raylib's SetShaderValue and GetShaderLocation
// functions to a single SetShaderValue method
class CShader
{
public:
    CShader() = default;
    virtual ~CShader()
    {
        Unload();
    };

    void Load(std::string vertexShader, std::string fragmentShader);
    void Unload();

    void SetShaderValue(std::string uniformName, std::any value, int type);

    Shader& GetShader();

private:
    std::unordered_map<std::string, int> m_ShaderLocs;
    Shader m_Shader{};
};

// The namespace Graphics contains all of the
namespace Graphics
{
    // Used to initialize a Raylib window
    bool InitWindow(Vec2f winDimensions, const char* title);

    // Used to close the Raylib window initialized by InitWindow
    bool CloseWindow();

    // Returns the width and height of the window in the form of a vector
    Vec2i GetWindowDimensions();

    // Before any internal GPU draw calls are done this function needs to be called
    void BeginDrawing();

    // Used to draw the contents from the buffer and flush it for new content
    void EndDrawing();

    // Used to set the window position
    void SetWindowPosition(Vec2f);
};

namespace Input
{
    // Checks if the passed key is pressed down
    bool IsKeyDown(int);

    // Checks if the passed key isn't pressed down
    bool IsKeyUp(int);
    
    // Returns mouse position (relative to the focused window) in the form of a vector
    Vec2i GetMousePos();

    // Returns the difference between the mouse position one frame ago and the current frame
    Vec2i GetMouseDelta();

    // Sets the mouse position relative to the window
    void SetMousePos(Vec2i);

    bool IsMouseButtonDown(int button);

    bool IsMouseButtonUp(int button);
    // float GetMouseWheelMove();
};