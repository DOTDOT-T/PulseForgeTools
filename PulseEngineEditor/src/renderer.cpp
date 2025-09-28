#include "renderer.h"
#include <glfw3.h>

Renderer::Renderer() {
    // Initialisation OpenGL (ex: configuration des buffers, etc.)
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Shader& shader) {
    // Applique le shader et dessine les objets 3D ici
    // glUseProgram(shader.getProgramID());
    // Dessin (par exemple, triangle, modèle, etc.)
}
