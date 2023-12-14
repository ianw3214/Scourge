#include "renderer.h"

// Temporary include to support texture loading
#include <unordered_map>

#include <gl/glew.h>

#include "shade/graphics/command/command.h"
#include "shade/graphics/texture.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"

// ======================================
// This file contains an OpenGL implementation of the renderer
// As things get abstracted, specific openGL constructs may need to be factored out
namespace {
    // ======================================
    GLuint VAO_rect;
    GLuint VBO_rect;
    GLuint VAO_line;
    GLuint VBO_line;
    // Can texture and rect share the same VAO/VBOs? Is it even worth it?
    GLuint VAO_texture;
    GLuint VBO_texture;

    // pixel space shader programs
    GLuint colourShaderProgram;
    GLuint textureShaderProgram;

    // normalized shader programs
    GLuint normalizedColourShaderProgram;  
    GLuint normalizedTextureShaderProgram;

    const char* pixelVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTextureCoord;\n"
        "uniform float ScreenWidth;\n"
        "uniform float ScreenHeight;\n"
        "out vec2 TextureCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4((aPos.x / ScreenWidth * 2.0) - 1.0, (aPos.y / ScreenHeight * 2.0) - 1.0, aPos.z, 1.0);\n"
        "   TextureCoord = aTextureCoord;\n"
        "}\0";  

    const char* normalizedVertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTextureCoord;\n"
        "out vec2 TextureCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "   TextureCoord = aTextureCoord;\n"
        "}\0";  

    const char* colourFragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "uniform vec4 InputColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = InputColor;\n"
        "}\0";

    const char* textureFragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TextureCoord;\n"
        "uniform sampler2D uTexture;\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(uTexture, TextureCoord);\n"
        "}\0";
}

// ======================================
Shade::RendererBase::RendererBase()
{

}

// ======================================
Shade::RendererBase::~RendererBase() = default;

// ======================================
void Shade::RendererBase::InitializeDefaultShaders()
{
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &pixelVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint normalizedVertexShader;
    normalizedVertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(normalizedVertexShader, 1, &normalizedVertexShaderSource, NULL);
    glCompileShader(normalizedVertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &colourFragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint textureShader;
    textureShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textureShader, 1, &textureFragmentShaderSource, NULL);
    glCompileShader(textureShader);

    colourShaderProgram = glCreateProgram();
    glAttachShader(colourShaderProgram, vertexShader);
    glAttachShader(colourShaderProgram, fragmentShader);
    glLinkProgram(colourShaderProgram);

    textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, vertexShader);
    glAttachShader(textureShaderProgram, textureShader);
    glLinkProgram(textureShaderProgram);

    normalizedColourShaderProgram = glCreateProgram();
    glAttachShader(normalizedColourShaderProgram, normalizedVertexShader);
    glAttachShader(normalizedColourShaderProgram, fragmentShader);
    glLinkProgram(normalizedColourShaderProgram);

    normalizedTextureShaderProgram = glCreateProgram();
    glAttachShader(normalizedTextureShaderProgram, normalizedVertexShader);
    glAttachShader(normalizedTextureShaderProgram, textureShader);
    glLinkProgram(normalizedTextureShaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(normalizedVertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(textureShader);

    {
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2,   // bottom left triangle
            1, 2, 3    // top right triangle triangle
        };  
        GLuint EBO;
        glGenVertexArrays(1, &VAO_rect);
        glGenBuffers(1, &VBO_rect);
        glBindVertexArray(VAO_rect);
        
        // Buffer data will be bound when drawing, no need to bind when initializing
        glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    {
        glGenVertexArrays(1, &VAO_line);
        glGenBuffers(1, &VBO_line);
        glBindVertexArray(VAO_line);
        
        // Buffer data will be bound when drawing, no need to bind when initializing
        glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    {
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 2,   // bottom left triangle
            1, 2, 3    // top right triangle triangle
        };  
        GLuint EBO;
        glGenVertexArrays(1, &VAO_texture);
        glGenBuffers(1, &VBO_texture);
        glBindVertexArray(VAO_texture);
        
        // Buffer data will be bound when drawing, no need to bind when initializing
        glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
}

// ======================================
void Shade::RendererBase::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// ======================================
void Shade::RendererBase::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

// ======================================
void Shade::RendererBase::SetWireframeMode(bool wireframeMode)
{
    glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
}

// ======================================
void Shade::RendererBase::QueueCommand(std::unique_ptr<RenderCommand> Command)
{
    mCommandQueue.emplace_back(std::move(Command));
}

// ======================================
void Shade::RendererBase::ProcessCommandQueue()
{
    for (std::unique_ptr<RenderCommand>& Command : mCommandQueue)
    {
        Command->Execute(this);
    }
}

// ======================================
void Shade::RendererBase::SwapCommandQueue(std::vector<std::unique_ptr<RenderCommand>>& NewCommandQueue)
{
    std::swap(mCommandQueue, NewCommandQueue);
}

// ======================================
void Shade::RendererBase::DrawLine(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour) const
{
    float vertices[] = {
        Point1x, Point1y, 0.0f,
        Point2x, Point2y, 0.0f
    };

    glUseProgram(colourShaderProgram);
    glBindVertexArray(VAO_line);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, LineColour.r, LineColour.g, LineColour.b, 1.f);
    // TODO: Find a better place to set these uniforms and make them not hard coded
    GLint screenWidthUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenWidth");
    glUniform1f(screenWidthUniformLocation, 1280.f);
    GLint screenHeightUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenHeight");
    glUniform1f(screenHeightUniformLocation, 720.f);
    glDrawArrays(GL_LINES, 0, 2);
}

// ======================================
void Shade::RendererBase::DrawLine(Vec2 Point1, Vec2 Point2, Colour LineColour) const
{
    DrawLine(Point1.x, Point1.y, Point2.x, Point2.y, LineColour);
}

// ======================================
void Shade::RendererBase::DrawLineNormalized(float Point1x, float Point1y, float Point2x, float Point2y, Colour LineColour) const
{
    float vertices[] = {
        Point1x, Point1y, 0.0f,
        Point2x, Point2y, 0.0f
    };

    glUseProgram(normalizedColourShaderProgram);
    glBindVertexArray(VAO_line);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, LineColour.r, LineColour.g, LineColour.b, 1.f);
    glDrawArrays(GL_LINES, 0, 2);
}

// ======================================
void Shade::RendererBase::DrawLineNormalized(Vec2 Point1, Vec2 Point2, Colour LineColour) const
{
    DrawLineNormalized(Point1.x, Point1.y, Point2.x, Point2.y, LineColour);
}

// ======================================
void Shade::RendererBase::DrawRectangle(float x, float y, float w, float h, Colour RectColour) const
{
    float vertices[] = {
        x,  y, 0.0f,  // bottom left
        x + w, y, 0.0f,  // bottom right
        x, y + h, 0.0f,  // top left
        x + w, y + h, 0.0f   // top right 
    };
    
    glUseProgram(colourShaderProgram);
    glBindVertexArray(VAO_rect);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, RectColour.r, RectColour.g, RectColour.b, 1.f);
    // TODO: Find a better place to set these uniforms and make them not hard coded
    GLint screenWidthUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenWidth");
    glUniform1f(screenWidthUniformLocation, 1280.f);
    GLint screenHeightUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenHeight");
    glUniform1f(screenHeightUniformLocation, 720.f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawRectangle(Vec2 pos, float w, float h, Colour RectColour) const
{
    DrawRectangle(pos.x, pos.y, w, h, RectColour);
}

// ======================================
void Shade::RendererBase::DrawRectangleNormalized(float x, float y, float w, float h, Colour RectColour) const
{
    float vertices[] = {
        x,  y, 0.0f,  // bottom left
        x + w, y, 0.0f,  // bottom right
        x, y + h, 0.0f,  // top left
        x + w, y + h, 0.0f   // top right 
    };
    
    glUseProgram(normalizedColourShaderProgram);
    glBindVertexArray(VAO_rect);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, RectColour.r, RectColour.g, RectColour.b, 1.f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawRectangleNormalized(Vec2 Position, float w, float h, Colour RectColour) const
{
    DrawRectangleNormalized(Position.x, Position.y, w, h, RectColour);
}

// ======================================
void Shade::RendererBase::DrawTexture(float x, float y, float w, float h, ResourceHandle TextureResource) const
{
    float vertices[] = {
        x,  y, 0.0f,        0.0, 1.0, // bottom left
        x + w, y, 0.0f,     1.0, 1.0, // bottom right
        x, y + h, 0.0f,     0.0, 0.0, // top left
        x + w, y + h, 0.0f, 1.0, 0.0  // top right 
    };
    
    ResourceManager* Manager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    Texture* DrawTexture = Manager->GetResource<Texture>(TextureResource);

    glUseProgram(textureShaderProgram);
    glBindVertexArray(VAO_texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    DrawTexture->BindTextureForRender();
    // TODO: Find a better place to set these uniforms and make them not hard coded
    GLint screenWidthUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenWidth");
    glUniform1f(screenWidthUniformLocation, 1280.f);
    GLint screenHeightUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenHeight");
    glUniform1f(screenHeightUniformLocation, 720.f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawTexture(Vec2 pos, float w, float h, ResourceHandle TextureResource) const
{
    DrawTexture(pos.x, pos.y, w, h, TextureResource);
}


// ======================================
void Shade::RendererBase::DrawTextureNormalized(float x, float y, float w, float h, ResourceHandle TextureResource) const
{
    float vertices[] = {
        x,  y, 0.0f,        0.0, 1.0, // bottom left
        x + w, y, 0.0f,     1.0, 1.0, // bottom right
        x, y + h, 0.0f,     0.0, 0.0, // top left
        x + w, y + h, 0.0f, 1.0, 0.0  // top right 
    };
    
    ResourceManager* Manager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    Texture* DrawTexture = Manager->GetResource<Texture>(TextureResource);

    glUseProgram(normalizedTextureShaderProgram);
    glBindVertexArray(VAO_texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    DrawTexture->BindTextureForRender();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Shade::RendererBase::DrawTextureNormalized(Vec2 pos, float w, float h, ResourceHandle TextureResource) const
{
    DrawTextureNormalized(pos.x, pos.y, w, h, TextureResource);
}