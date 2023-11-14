#pragma once

#include <string>

#include "shade/graphics/command/command.h"

#include "shade/common/vec.h"

namespace Shade {

    class DrawTextureCommand : public RenderCommand {
    public:
        DrawTextureCommand();
        DrawTextureCommand(Vec2 Position, float Width, float Height);
        DrawTextureCommand(Vec2 Position, float Width, float Height, const std::string& TexturePath);
        DrawTextureCommand(Vec2 Position, float Width, float Height, const std::string& TexturePath, bool Normalized);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, const std::string& TexturePath);
        DrawTextureCommand(float xPosition, float yPosition, float Width, float Height, const std::string& TexturePath, bool Normalized);

        virtual void Execute(RendererBase* Renderer) override;
    private:
            Vec2 mPosition;
            float mWidth = 1.f;
            float mHeight = 1.f;
            std::string mTexturePath;
            bool mNormalized = false;
    };

}