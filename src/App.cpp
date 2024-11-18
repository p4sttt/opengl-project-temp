#include "App.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cmath>
#include <glad.h>

App::Window::Window() : _window(nullptr), _renderer(new Graphics::Renderer())
{
    if (!glfwInit())
    {
        LOG_ERROR << "Initialize GLFW failed" << '\n';
    }

    _window = glfwCreateWindow(500, 500, "OpenGL Ultimate Window", NULL, NULL);
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

    EventHandler::Initialize(this->_window);
}

App::Window::~Window()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
    EventHandler::Shutdown();

    if (_renderer)
    {
        delete _renderer;
        _renderer = nullptr;
    }

    LOG << "Window killed" << '\n';
}

Graphics::Renderer* App::Window::GetRenderer()
{
    return _renderer;
}

void App::Window::SetRenderer(Graphics::Renderer* renderer)
{
    if (_renderer)
    {
        delete _renderer;
        _renderer = nullptr;
    }
    _renderer = renderer;
}

void App::Window::Render()
{
    TransformController defaultTransformController(_renderer->GetModel()->GetTransform());
    EventHandler handler;
    handler.AddListener(&defaultTransformController);

    LOG << "Start render loop" << '\n';
    while (!glfwWindowShouldClose(_window))
    {
        _renderer->Render();
        glfwSwapBuffers(_window);
        EventHandler::PollEvents();
    }
}

void App::EventHandler::Initialize(GLFWwindow* window)
{
    LOG << "Initialize EventHandler" << '\n';
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
}

void App::EventHandler::Shutdown()
{
    _listeners.clear();
    LOG << "Erase all EventHandlers" << '\n';
}

void App::EventHandler::KeyCallback(
    GLFWwindow* window, int key, int scancode, int action, int mods
)
{
    for (auto listener : _listeners)
    {
        listener->OnKeyEvent(key, action, mods);
    }
}

void App::EventHandler::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void App::EventHandler::AddListener(IEventListener* listener)
{
    LOG << "Add new EventListener: " << listener << '\n';
    if (listener)
        _listeners.push_back(listener);
}

void App::EventHandler::RemoveListener(IEventListener* listener)
{
    LOG << "Remove EventListeber: " << listener << '\n';
    _listeners.erase(
        std::remove(_listeners.begin(), _listeners.end(), listener), _listeners.end()
    );
}

void App::EventHandler::PollEvents()
{
    glfwPollEvents();
}

std::vector<App::IEventListener*> App::EventHandler::_listeners;

void App::TransformController::OnKeyEvent(int key, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        if (key == GLFW_KEY_UP)
            if (mods & GLFW_MOD_CONTROL)
                _transform->Rotate(Math::Axis::x, true);
            else
                _transform->Move(Math::Axis::y, true);

        else if (key == GLFW_KEY_RIGHT)
            if (mods & GLFW_MOD_CONTROL)
                _transform->Rotate(Math::Axis::y, false);
            else
                _transform->Move(Math::Axis::x, true);

        else if (key == GLFW_KEY_DOWN)
            if (mods & GLFW_MOD_CONTROL)
                _transform->Rotate(Math::Axis::x, false);
            else
                _transform->Move(Math::Axis::y, false);

        else if (key == GLFW_KEY_LEFT)
            if (mods & GLFW_MOD_CONTROL)
                _transform->Rotate(Math::Axis::y, true);
            else
                _transform->Move(Math::Axis::x, false);

        if (key == GLFW_KEY_LEFT_BRACKET)
            _transform->Scale(Math::Axis::x, false);
        if (key == GLFW_KEY_RIGHT_BRACKET)
            _transform->Scale(Math::Axis::x, true);
    }
    else if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_R)
            _transform->Reset();
    }
}

App::TransformController::TransformController(Math::Transform* transform)
    : _transform(transform)
{
    LOG << "TransformController created" << '\n';
}

App::TransformController::~TransformController()
{

    LOG << "Kill TransformController" << '\n';
}
