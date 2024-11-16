#include "App.hpp"
#include "Graphics.hpp"
#include "Logger.hpp"
#include "ProjectLoader.hpp"
#include <glad.h>

void OnQKeyPressed()
{
    LOG << "Q key pressed!" << '\n';
}

int main()
{
    App::Window* window = new App::Window();
    App::EventHandler* eventHandler = new App::EventHandler(window->GetGLFWWindow());
    Graphics::Renderer* renderer = new Graphics::Renderer();
    ProjectLoader& loader = ProjectLoader::GetInstance();

    Graphics::Model* model = loader.LoadModel();
    Graphics::ShaderProgram* shaderProgram = loader.LoadShaderProgram();

    renderer->SetModel(model);
    renderer->SetShaderProgram(shaderProgram);
    window->SetRenderer(renderer);
    eventHandler->RegisterKeyCallback(GLFW_KEY_Q, OnQKeyPressed);

    window->Render();

    delete renderer;
    delete window;
    return 0;
}