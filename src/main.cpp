#include "App.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "ProjectLoader.hpp"
#include <glad.h>

int main()
{
    App::Window* window = new App::Window();
    ProjectLoader& loader = ProjectLoader::GetInstance();

    Graphics::Model* model = loader.LoadModel();
    Graphics::ShaderProgram* shaderProgram = loader.LoadShaderProgram();

    window->GetRenderer()->SetModel(model);
    window->GetRenderer()->SetShaderProgram(shaderProgram);

    window->Render();

    delete window;
    return 0;
}