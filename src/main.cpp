#include "App.hpp"
#include "Graphics.hpp"
#include "ProjectLoader.hpp"
#include <glad.h>

// Сейчас код не работает, так как есть ошибки в ProjectLoader
// но если захардкодить вершины, индексы и шейдеры, то все будет ок

int main()
{
    App::Window* window = new App::Window();
    Graphics::Renderer* renderer = new Graphics::Renderer();
    ProjectLoader& loader = ProjectLoader::GetInstance();

    Graphics::Model* model = loader.LoadModel();
    Graphics::ShaderProgram* shaderProgram = loader.LoadShaderProgram();

    renderer->SetModel(model);
    renderer->SetShaderProgram(shaderProgram);
    window->SetRenderer(renderer);

    window->Render();

    delete renderer;
    delete window;
    return 0;
}
