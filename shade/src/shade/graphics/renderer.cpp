#include "renderer.h"

// Temporary include to support texture loading
#include <unordered_map>

#include <gl/glew.h>

#include "shade/graphics/camera/camera.h"
#include "shade/graphics/command/command.h"
#include "shade/graphics/texture.h"
#include "shade/instance/service/provider.h"
#include "shade/resource/manager.h"

// ======================================
// This file contains an OpenGL implementation of the renderer
// As things get abstracted, specific openGL constructs may need to be factored out

// ======================================
namespace {
    GLuint VAO_rect;
    GLuint VBO_rect;
    GLuint VAO_line;
    GLuint VBO_line;
    GLuint VAO_circle;
    GLuint VBO_circle;
    // Can texture and rect share the same VAO/VBOs? Is it even worth it?
    GLuint VAO_texture;
    GLuint VBO_texture;

    // pixel space shader programs
    GLuint colourShaderProgram;
    GLuint textureShaderProgram;

    // 2D pixel space shader program w/ depth
    GLuint colourShaderProgram2D;

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

    const char* pixelVertexShaderSourceWithDepth2D = "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTextureCoord;\n"
        "uniform float ScreenWidth;\n"
        "uniform float ScreenHeight;\n"
        "uniform float Depth;\n"
        "out vec2 TextureCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4((aPos.x / ScreenWidth * 2.0) - 1.0, (aPos.y / ScreenHeight * 2.0) - 1.0, Depth, 1.0);\n"
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
        "uniform vec3 uColourMultiplier;\n"
        "void main()\n"
        "{\n"
        "    FragColor = InputColor * vec4(uColourMultiplier, 1.0);\n"
        "}\0";

    const char* textureFragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TextureCoord;\n"
        "uniform sampler2D uTexture;\n"
        "uniform vec3 uColourMultiplier;\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(uTexture, TextureCoord) * vec4(uColourMultiplier, 1.0);\n"
        "}\0";
}

// ======================================
Shade::RendererBase::RendererBase()
{
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.9f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);  
    glDepthFunc(GL_LEQUAL);
}

// ======================================
Shade::RendererBase::~RendererBase() = default;

// ======================================
// TODO: Need to handle shader compilation errors
//  - Probably just want to crash, since shaders should never fail to compile
void Shade::RendererBase::InitializeDefaultShaders()
{
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &pixelVertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint vertexShader2D;
    vertexShader2D = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader2D, 1, &pixelVertexShaderSourceWithDepth2D, NULL);
    glCompileShader(vertexShader2D);

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
    {
        glUseProgram(colourShaderProgram);
        GLint screenWidthUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenWidth");
        glUniform1f(screenWidthUniformLocation, 1280.f);
        GLint screenHeightUniformLocation = glGetUniformLocation(colourShaderProgram, "ScreenHeight");
        glUniform1f(screenHeightUniformLocation, 720.f);
    }

    textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, vertexShader);
    glAttachShader(textureShaderProgram, textureShader);
    glLinkProgram(textureShaderProgram);
    {
        glUseProgram(textureShaderProgram);
        GLint screenWidthUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenWidth");
        glUniform1f(screenWidthUniformLocation, 1280.f);
        GLint screenHeightUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenHeight");
        glUniform1f(screenHeightUniformLocation, 720.f);
    }

    colourShaderProgram2D = glCreateProgram();
    glAttachShader(colourShaderProgram2D, vertexShader2D);
    glAttachShader(colourShaderProgram2D, fragmentShader);
    glLinkProgram(colourShaderProgram2D);
    {
        glUseProgram(colourShaderProgram2D);
        GLint screenWidthUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenWidth");
        glUniform1f(screenWidthUniformLocation, 1280.f);
        GLint screenHeightUniformLocation = glGetUniformLocation(textureShaderProgram, "ScreenHeight");
        glUniform1f(screenHeightUniformLocation, 720.f);
    }

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
        glGenVertexArrays(1, &VAO_circle);
        glGenBuffers(1, &VBO_circle);
        glBindVertexArray(VAO_circle);

        // Buffer data will be bound when drawing, no need to bind when initializing
        glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
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
void Shade::RendererBase::QueueCommand(std::unique_ptr<RenderCommand> command)
{
    mCommandQueue.emplace_back(std::move(command));
}

// ======================================
void Shade::RendererBase::ProcessCommandQueue()
{
    CameraService* camera = ServiceProvider::GetCurrentProvider()->GetService<CameraService>();
    for (std::unique_ptr<RenderCommand>& command : mCommandQueue)
    {
        command->Execute(this, camera->GetCameraInfo());
    }
}

// ======================================
void Shade::RendererBase::SwapCommandQueue(std::vector<std::unique_ptr<RenderCommand>>& newCommandQueue)
{
    std::swap(mCommandQueue, newCommandQueue);
}

// ======================================
void Shade::RendererBase::DrawLine(float point1x, float point1y, float point2x, float point2y, Colour colour, float depth) const
{
    float vertices[] = {
        point1x, point1y, depth,
        point2x, point2y, depth
    };

    glUseProgram(colourShaderProgram);
    glBindVertexArray(VAO_line);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(colourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawArrays(GL_LINES, 0, 2);
}

// ======================================
void Shade::RendererBase::DrawLine(Vec2 point1, Vec2 point2, Colour colour, float depth) const
{
    DrawLine(point1.x, point1.y, point2.x, point2.y, colour, depth);
}

// ======================================
void Shade::RendererBase::DrawLineNormalized(float point1x, float point1y, float point2x, float point2y, Colour colour, float depth) const
{
    float vertices[] = {
        point1x, point1y, depth,
        point2x, point2y, depth
    };

    glUseProgram(normalizedColourShaderProgram);
    glBindVertexArray(VAO_line);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawArrays(GL_LINES, 0, 2);
}

// ======================================
void Shade::RendererBase::DrawLineNormalized(Vec2 point1, Vec2 point2, Colour colour, float depth) const
{
    DrawLineNormalized(point1.x, point1.y, point2.x, point2.y, colour, depth);
}

// ======================================
void Shade::RendererBase::DrawLines(std::vector<Vec2> points, Colour colour, float depth) const
{
    // TODO: Consider optimizing this
    //  - How can this be cached to avoid creating/deleting VAOs and VBOs all the time
    //  - Doesn't look like buffer size needs to be known at creation time, perhaps can follow same pattern as other primitives
    GLuint VAO_lines;
    GLuint VBO_lines;
    glGenVertexArrays(1, &VAO_lines);
    glGenBuffers(1, &VBO_lines);
    glBindVertexArray(VAO_lines);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_lines);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2) * points.size(), &points[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glUseProgram(colourShaderProgram2D);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram2D, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint depthUniformLocation = glGetUniformLocation(colourShaderProgram2D, "Depth");
    glUniform1f(depthUniformLocation, depth);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(colourShaderProgram2D, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawArrays(GL_LINE_STRIP, 0, points.size());

    glDeleteBuffers(1, &VBO_lines);
    glDeleteVertexArrays(1, &VAO_lines);
}

// ======================================
void Shade::RendererBase::DrawLinesNormalized(std::vector<Vec2> points, Colour colour, float depth) const
{
    // TODO: Implement
}

// ======================================
void Shade::RendererBase::DrawCircle(float x, float y, float radius, Colour colour, float depth) const
{
    constexpr float  PI_F = 3.14159265358979f;
    constexpr size_t NUM_SEGMENTS = 16;
    constexpr float ANGLE_OFFSET = 2.f * PI_F / static_cast<float>(NUM_SEGMENTS);

    float vertices[NUM_SEGMENTS * 3];
    for (size_t i = 0; i < NUM_SEGMENTS; ++i)
    {
        vertices[i * 3] = x + radius * std::cos(ANGLE_OFFSET * i);
        vertices[i * 3 + 1] = y + radius * std::sin(ANGLE_OFFSET * i);
        vertices[i * 3 + 2] = depth;
    }

    glUseProgram(colourShaderProgram);
    glBindVertexArray(VAO_circle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * NUM_SEGMENTS, vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(colourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawArrays(GL_LINE_LOOP, 0, NUM_SEGMENTS);
}

// ======================================
void Shade::RendererBase::DrawCircleNormalized(float x, float y, float radius, Colour colour, float depth) const
{
    // This scales in a janky way, might not make sense for a normalize version of this to exist
    constexpr float  PI_F = 3.14159265358979f;
    constexpr size_t NUM_SEGMENTS = 16;
    constexpr float ANGLE_OFFSET = 2.f * PI_F / static_cast<float>(NUM_SEGMENTS);

    float vertices[NUM_SEGMENTS * 3];
    for (size_t i = 0; i < NUM_SEGMENTS; ++i)
    {
        vertices[i * 3] = x + radius * std::cos(ANGLE_OFFSET * i);
        vertices[i * 3 + 1] = y + radius * std::sin(ANGLE_OFFSET * i);
        vertices[i * 3 + 2] = depth;
    }

    glUseProgram(normalizedColourShaderProgram);
    glBindVertexArray(VAO_circle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * NUM_SEGMENTS, vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawArrays(GL_LINE_LOOP, 0, NUM_SEGMENTS);
}

// ======================================
void Shade::RendererBase::DrawRectangle(float x, float y, float w, float h, Colour colour, float depth) const
{
    float vertices[] = {
        x,  y, depth,       // bottom left
        x + w, y, depth,    // bottom right
        x, y + h, depth,    // top left
        x + w, y + h, depth // top right 
    };
    
    glUseProgram(colourShaderProgram);
    glBindVertexArray(VAO_rect);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(colourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(colourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawRectangle(Vec2 pos, float w, float h, Colour colour, float depth) const
{
    DrawRectangle(pos.x, pos.y, w, h, colour, depth);
}

// ======================================
void Shade::RendererBase::DrawRectangleNormalized(float x, float y, float w, float h, Colour colour, float depth) const
{
    float vertices[] = {
        x,  y, depth,       // bottom left
        x + w, y, depth,    // bottom right
        x, y + h, depth,    // top left
        x + w, y + h, depth // top right 
    };
    
    glUseProgram(normalizedColourShaderProgram);
    glBindVertexArray(VAO_rect);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_rect);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLint positionUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "InputColor");
    glUniform4f(positionUniformLocation, colour.r, colour.g, colour.b, 1.f);
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(normalizedColourShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawRectangleNormalized(Vec2 position, float w, float h, Colour colour, float depth) const
{
    DrawRectangleNormalized(position.x, position.y, w, h, colour, depth);
}

// ======================================
void Shade::RendererBase::DrawTexture(float x, float y, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth) const
{
    float vertices[] = {
        x,  y, depth,           textureSource.x, textureSource.y + textureSource.h, // bottom left
        x + w, y, depth,        textureSource.x + textureSource.w, textureSource.y + textureSource.h, // bottom right
        x, y + h, depth,        textureSource.x, textureSource.y, // top left
        x + w, y + h, depth,    textureSource.x + textureSource.w, textureSource.y  // top right 
    };
    
    ResourceManager* Manager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    Texture* DrawTexture = Manager->GetResource<Texture>(textureResource);

    glUseProgram(textureShaderProgram);
    glBindVertexArray(VAO_texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    DrawTexture->BindTextureForRender();
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(textureShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawTexture(Vec2 pos, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth) const
{
    DrawTexture(pos.x, pos.y, w, h, textureResource, textureSource, depth);
}


// ======================================
void Shade::RendererBase::DrawTextureNormalized(float x, float y, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth) const
{
    float vertices[] = {
        x,  y, depth,           textureSource.x, textureSource.y + textureSource.h, // bottom left
        x + w, y, depth,        textureSource.x + textureSource.w, textureSource.y + textureSource.h, // bottom right
        x, y + h, depth,        textureSource.x, textureSource.y, // top left
        x + w, y + h, depth,    textureSource.x + textureSource.w, textureSource.y  // top right 
    };
    
    ResourceManager* Manager = ServiceProvider::GetCurrentProvider()->GetService<ResourceManager>();
    Texture* DrawTexture = Manager->GetResource<Texture>(textureResource);

    glUseProgram(normalizedTextureShaderProgram);
    glBindVertexArray(VAO_texture);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    DrawTexture->BindTextureForRender();
    GLint uColourMultiplierUniformLocation = glGetUniformLocation(normalizedTextureShaderProgram, "uColourMultiplier");
    glUniform3f(uColourMultiplierUniformLocation, mColourMultiplier.r, mColourMultiplier.g, mColourMultiplier.b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// ======================================
void Shade::RendererBase::DrawTextureNormalized(Vec2 pos, float w, float h, ResourceHandle textureResource, textureSourceInfo textureSource, float depth) const
{
    DrawTextureNormalized(pos.x, pos.y, w, h, textureResource, textureSource, depth);
}

// ======================================
void Shade::RendererBase::SetColourMultiplier(Colour colour)
{
    mColourMultiplier = colour;
}

// ======================================
void Shade::RendererBase::ResetColourMultiplier()
{
    mColourMultiplier = Colour{ 1.f, 1.f, 1.f };
}
