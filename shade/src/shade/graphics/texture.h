#pragma once

// I don't like having this here... might want to move to platform specific layer
#include <gl/glew.h>

#include "shade/resource/resource.h"

namespace Shade {
    
    class Texture : public Resource {
    public:
        static Resource* Load(const std::string& path);

    public:
        Texture(GLuint textureID, int width, int height);
    
        int GetWidth() const;
        int GetHeight() const;

        void BindTextureForRender();
    private:
        GLuint mGLTextureID;
        // The raw width/height of the original loaded texture in pixels
        int mWidth = 0;
        int mHeight = 0;
    };

}