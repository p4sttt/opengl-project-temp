#include "App.hpp"
#include "Graphics.hpp"
#include "Math.hpp"
#include "ProjectLoader.hpp"
#include <glad.h>
#include <vector>

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