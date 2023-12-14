#include "texture.h"

// The opengl specific stuff could probably be moved to a more platform specific thing as well?
//  - needs more investigation + planning
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ======================================
Shade::Resource* Shade::Texture::Load(const std::string& Path)
{
    GLuint TextureID;
    // Temporarily generating texture data here
    int width, height, numChannels;
    unsigned char* data = stbi_load(Path.c_str(), &width, &height, &numChannels, 0);
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(data);

    return new Texture(TextureID);
}

// ======================================
Shade::Texture::Texture(GLuint TextureID)
    : mGLTextureID(TextureID)
{

}

// ======================================
void Shade::Texture::BindTextureForRender()
{
    glBindTexture(GL_TEXTURE_2D, mGLTextureID);
}