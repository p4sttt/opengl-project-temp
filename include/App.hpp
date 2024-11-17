#pragma once

#include "Graphics.hpp"
#define GLFW_INCLUDE_NONE
#include "Math.hpp"
#include <GLFW/glfw3.h>
#include <map>

using KeyCombination = std::pair<int, int>;

namespace App
{

class Window
{
  private:
    GLFWwindow* _window;
    Graphics::Renderer* _renderer;

  public:
    Window();
    ~Window();

    Graphics::Renderer* GetRenderer();
    void SetRenderer(Graphics::Renderer* renderer);

    void Render();
};

class IEventListener
{
  public:
    virtual void OnKeyEvent(int key, int action, int mods) = 0;
    virtual ~IEventListener() = default;
};

class EventHandler
{
  private:
    static std::vector<IEventListener*> _listeners;

    static void KeyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mods
    );
    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

  public:
    static void Initialize(GLFWwindow* window);
    static void Shutdown();

    void AddListener(IEventListener* listener);
    void RemoveListener(IEventListener* listender);

    static void PollEvents();
};

class TransformController : public IEventListener
{
  private:
    Math::Transform _transform;

  public:
    TransformController(Math::Transform& transform);
    ~TransformController();

    void OnKeyEvent(int key, int action, int mods) override;
};

} // namespace App