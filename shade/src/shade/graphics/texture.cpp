#include "texture.h"

// The opengl specific stuff could probably be moved to a more platform specific thing as well?
//  - needs more investigation + planning
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ======================================
Shade::Resource* Shade::Texture::Load(const std::string& path)
{
    GLuint textureID;
    // Temporarily generating texture data here
    // TODO: Move this to a file system
    int width, height, numChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &numChannels, 0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    stbi_image_free(data);

    return new Texture(textureID);
}

// ======================================
Shade::Texture::Texture(GLuint textureID)
    : mGLTextureID(textureID)
{

}

// ======================================
void Shade::Texture::BindTextureForRender()
{
    glBindTexture(GL_TEXTURE_2D, mGLTextureID);
}