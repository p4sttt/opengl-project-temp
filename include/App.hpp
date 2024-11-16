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

    void Render();
};

} // namespace App
