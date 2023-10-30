#include "renderer.h"

#include <gl/glew.h>

// ======================================
Shade::Renderer::~Renderer()
{

}

// ======================================
void Shade::Renderer::Clear()
{
    // glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}