#pragma once

// I don't like having this here... might want to move to platform specific layer
#include <gl/glew.h>

#include "shade/resource/resource.h"

namespace Shade {
    
    class Texture : public Resource {
    public:
        static Resource* Load(const std::string& Path);

    public:
        Texture(GLuint TextureID);
    
        void BindTextureForRender();
    private:
        GLuint mGLTextureID;
    };

}