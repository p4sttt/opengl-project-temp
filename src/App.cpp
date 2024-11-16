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

GLFWwindow* App::Window::GetGLFWWindow()
{
    return _window;
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

App::EventHandler::EventHandler(GLFWwindow* window) : _window(window)
{
    glfwSetWindowUserPointer(_window, this);
    glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);
    glfwSetKeyCallback(_window, KeyCallback);
    glfwSetMouseButtonCallback(_window, MouseButtonCallback);
}

App::EventHandler::~EventHandler()
{
    glfwSetKeyCallback(_window, nullptr);
    glfwSetMouseButtonCallback(_window, nullptr);
}

void App::EventHandler::RegisterKeyCallback(int key, Callback callback)
{
    _key_callbacks[key] = callback;
}

void App::EventHandler::RegisterMouseButtonCallback(int button, Callback callback)
{
    _mouse_button_callbacks[button] = callback;
}

void App::EventHandler::UnregisterKeyCallback(int key)
{
    _key_callbacks.erase(key);
}

void App::EventHandler::UnregisterMouseButtonCallback(int button)
{
    _mouse_button_callbacks.erase(button);
}

void App::EventHandler::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void App::EventHandler::KeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods
)
{
    EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
    if (handler && action == GLFW_PRESS)
    {
        auto it = handler->_key_callbacks.find(key);
        if (it != handler->_key_callbacks.end())
        {
            it->second();
        }
    }
}

void App::EventHandler::MouseButtonCallback(
    GLFWwindow* window, int button, int action, int mods
)
{
    EventHandler* handler = static_cast<EventHandler*>(glfwGetWindowUserPointer(window));
    if (handler && action == GLFW_PRESS)
    {
        auto it = handler->_mouse_button_callbacks.find(button);
        if (it != handler->_mouse_button_callbacks.end())
        {
            it->second();
        }
    }
}