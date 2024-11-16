#pragma once

#include "Graphics.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <functional>
#include <map>

using Callback = std::function<void()>;
using KeyCombination = std::pair<int, int>;

namespace App
{

class Window
{
  private:
    GLFWwindow* _window;
    Graphics::Renderer* _renderer;
    std::map<KeyCombination, Callback> _callback_map;

  public:
    Window();
    ~Window();

    void SetRenderer(Graphics::Renderer* renderer);
    GLFWwindow* GetGLFWWindow();

    void Render();
};

class EventHandler
{
  private:
    GLFWwindow* _window;
    std::map<int, Callback> _key_callbacks;
    std::map<int, Callback> _mouse_button_callbacks;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(
        GLFWwindow* window, int key, int scancode, int action, int mods
    );
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

  public:
    EventHandler(GLFWwindow* window);
    ~EventHandler();

    void RegisterKeyCallback(int key, Callback callback);
    void RegisterMouseButtonCallback(int button, Callback callback);
    void UnregisterKeyCallback(int key);
    void UnregisterMouseButtonCallback(int button);
};

} // namespace App
