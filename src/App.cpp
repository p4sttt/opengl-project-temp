#include "App.hpp"
#include "Logger.hpp"
#include <glad.h>
#include <iostream>

App::Window::Window() : _window(nullptr), _renderer(nullptr)
{
    if (!glfwInit())
    {
        LOG_ERROR << "Initialize GLFW failed" << '\n';
    }

    _window = glfwCreateWindow(640, 480, "This is GLFW Window", NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        LOG_ERROR << "Creation window failed" << '\n';
    }

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR << "Failed to initialize GLAD" << '\n';
    }
    else
    {
        LOG << "Window successfully created" << '\n';
    }
}

App::Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();

    LOG << "Window killed" << '\n';
}

void App::Window::SetRenderer(Graphics::Renderer* renderer)
{
    delete _renderer;
    _renderer = renderer;
}

void App::Window::Render()
{
    LOG << "Start render loop" << '\n';

    while (!glfwWindowShouldClose(_window))
    {
        _renderer->Render();

        glfwSwapBuffers(_window);

        glfwPollEvents();
    }
}